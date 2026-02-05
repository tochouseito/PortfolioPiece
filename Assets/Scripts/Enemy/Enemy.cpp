// Enemy.cpp
#include "Enemy.h"
using namespace theatriaSystem;

#include "Enemy/EnemySpawner.h"
#include "UI/Target.h"
#include "UI/LockOn.h"
#include "Player/Player.h"
#include "Generator/Generator.h"

#include <cmath>
#include <algorithm>
#include <functional>
#include <random>

namespace
{
    constexpr float kPi = 3.14159265358979323846f;

    float clamp01(float v) noexcept
    {
        return std::clamp(v, 0.0f, 1.0f);
    }

    float deg_to_rad(float deg) noexcept
    {
        return deg * (kPi / 180.0f);
    }

    float rad_to_deg(float rad) noexcept
    {
        return rad * (180.0f / kPi);
    }

    float length3(const math::float3& v) noexcept
    {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    math::float3 normalize3(const math::float3& v) noexcept
    {
        const float len = length3(v);
        if (len <= 1e-6f)
        {
            return math::float3(0.0f, 0.0f, 1.0f);
        }

        return math::float3(v.x / len, v.y / len, v.z / len);
    }

    math::float3 lerp3(const math::float3& a, const math::float3& b, float t) noexcept
    {
        return math::float3(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t
        );
    }
}

void Enemy::Start()
{
    // 1) 参照取得
    m_EnemySpawner = GetMarionnette<EnemySpawner>(L"EnemySpawner");
    m_Target = GetMarionnette<Target>(L"Target");
    m_Player = GetMarionnette<Player>(L"Player");
    m_Generator = GetMarionnette<Generator>(L"Generator");

    // 2) タグ設定
    gameObject.SetTag("Enemy");

    // 3) 乱数 seed（名前から作る）
    {
        const uint32_t seed = static_cast<uint32_t>(std::hash<std::wstring>{}(gameObject.GetName()));
        m_Rng.seed(seed);
    }

    // 4) ランダム旋回初期化
    {
        m_RandomTimer = 0.0f;
        m_MoveDir = math::float3(0.0f, 0.0f, 1.0f);
        m_TargetDir = math::float3(0.0f, 0.0f, 1.0f);
    }

    // 5) ここで isActive=false なら何もしない
    if (!m_IsActive)
    {
        return;
    }

    // 6) 攻撃パターンを決定
    {
        std::uniform_int_distribution<int> patternDist(0, 2);
        m_AttackPattern = static_cast<AttackPattern>(patternDist(m_Rng));

        if (m_AttackPattern == AttackPattern::Burst)
        {
            std::uniform_real_distribution<float> offset(0.0f, m_BurstInterval);
            m_AttackTimer = offset(m_Rng);
        }
        else if (m_AttackPattern == AttackPattern::HomingMissile)
        {
            std::uniform_real_distribution<float> offset(0.0f, m_MissileInterval);
            m_AttackTimer = offset(m_Rng);
        }
    }
}

void Enemy::Update()
{
    // 1) 死亡中アニメーションを優先
    if (m_IsDying)
    {
        DeadAnimation();
        return;
    }

    // 2) 非アクティブなら何もしない
    if (!m_IsActive)
    {
        return;
    }

    // 3) 移動
    Move();

    // 4) 攻撃
    UpdateAttack();
}

void Enemy::EnableLockOnTarget(LockOn* lockOn)
{
    // 1) 状態更新
    m_IsLockOnTarget = true;
    m_LockOn = lockOn;
}

void Enemy::SetApproachTarget(const math::float3& target, float arriveRadius)
{
    m_ApproachTarget = target;
    m_ApproachArriveRadius = arriveRadius;
    m_IsApproaching = true;
}

void Enemy::UnableLockOnTarget()
{
    // 1) ロックオン対象でなければ何もしない
    if (!m_IsLockOnTarget)
    {
        return;
    }

    // 2) 状態更新
    m_IsLockOnTarget = false;

    // 3) LockOn UI 側から登録解除
    if (m_Target && m_LockOn)
    {
        m_Target->RemoveLockOn(m_LockOn->gameObject.GetName());
    }

    // 4) LockOn UI 自体を破棄
    if (m_LockOn)
    {
        DestroyGameObject(&m_LockOn->gameObject);
        m_LockOn = nullptr;
    }
}

// 衝突関数
void Enemy::OnCollisionEnter(GameObject& other)
{
    // 1) プレイヤー攻撃以外は無視
    if (other.GetTag() != "PlayerAttack")
    {
        if (other.GetTag() == "Player")
        {
            if (m_IsDying)
            {
                return;
            }

            if (m_EnemySpawner)
            {
                m_EnemySpawner->RemoveEnemy(gameObject.GetName());
            }

            UnableLockOnTarget();
            BeginDead();
        }
        return;
    }

    // 2) すでに死亡中なら二重処理しない
    if (m_IsDying)
    {
        return;
    }

    // 3) スポーン管理からはここで抜く（ロジック的にはもう”死んだ”扱い）
    if (m_EnemySpawner)
    {
        m_EnemySpawner->RemoveEnemy(gameObject.GetName());
    }

    // 4) 今ロックオンされていたら解除
    UnableLockOnTarget();

    // 5) 即 Destroy ではなく、死亡アニメ開始
    BeginDead();
}

void Enemy::Move()
{
    // 1) dt
    const float dt = DeltaTime();

    if (m_IsRamming && m_Player)
    {
        const math::float3 toPlayer = m_Player->transform->position - transform->position;
        const math::float3 dir = normalize3(toPlayer);
        m_Velocity = dir * m_RamSpeed;

        Rigidbody3D rb = GetComponent<Rigidbody3D>();
        if (rb)
        {
            rb->velocity = m_Velocity;
        }

        auto transformComp = GetComponent<Transform>();
        if (transformComp)
        {
            const float yaw = std::atan2(dir.x, dir.z);
            const float pitch = -std::atan2(dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));
            transformComp->degrees.y = rad_to_deg(yaw);
            transformComp->degrees.x = rad_to_deg(pitch);
            transformComp->degrees.z = -rad_to_deg(yaw) * 0.2f;
        }

        if (transform->position.y < m_MinAltitude)
        {
            transform->position.y = m_MinAltitude;
            if (m_Velocity.y < 0.0f)
            {
                m_Velocity.y = 0.0f;
                if (rb)
                {
                    rb->velocity = m_Velocity;
                }
            }
        }
        return;
    }

    // 1.5) 生成直後のアプローチ移動
    if (m_IsApproaching)
    {
        const math::float3 toTarget = m_ApproachTarget - transform->position;
        const float distance = length3(toTarget);
        if (distance <= m_ApproachArriveRadius)
        {
            m_IsApproaching = false;
        }
        else
        {
            const math::float3 dir = normalize3(toTarget);
            m_Velocity = dir * m_ApproachSpeed;

            Rigidbody3D rb = GetComponent<Rigidbody3D>();
            if (rb)
            {
                rb->velocity = m_Velocity;
            }

            auto transformComp = GetComponent<Transform>();
            if (transformComp)
            {
                const float yaw = std::atan2(dir.x, dir.z);
                const float pitch = -std::atan2(dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));
                transformComp->degrees.y = rad_to_deg(yaw);
                transformComp->degrees.x = rad_to_deg(pitch);
                transformComp->degrees.z = -rad_to_deg(yaw) * 0.2f;
            }
            if (transform->position.y < m_MinAltitude)
            {
                transform->position.y = m_MinAltitude;
                if (m_Velocity.y < 0.0f)
                {
                    m_Velocity.y = 0.0f;
                    if (rb)
                    {
                        rb->velocity = m_Velocity;
                    }
                }
            }
            return;
        }
    }

    // 2) 3秒ごとにランダム目標方向を更新
    m_RandomTimer += dt;
    if (m_RandomTimer >= m_RandomIntervalSec)
    {
        // 1) タイマー巻き戻し（dtが大きくても破綻しない）
        m_RandomTimer -= m_RandomIntervalSec;

        // 2) ランダム角度生成（Yaw/Pitch）
        std::uniform_real_distribution<float> yawDist(-m_RandomYawRangeDeg, m_RandomYawRangeDeg);
        std::uniform_real_distribution<float> pitchDist(-m_RandomPitchRangeDeg, m_RandomPitchRangeDeg);

        const float yaw = deg_to_rad(yawDist(m_Rng));
        const float pitch = deg_to_rad(pitchDist(m_Rng));

        // 3) yaw/pitch → 方向ベクトル（前方基準）
        //    x: 右, y: 上, z: 前（あなたの既存コードが z 前進なのでこれ）
        const float cy = std::cos(yaw);
        const float sy = std::sin(yaw);
        const float cp = std::cos(pitch);
        const float sp = std::sin(pitch);

        m_TargetDir = normalize3(math::float3(
            sy * cp, // x
            sp,      // y
            cy * cp  // z
        ));
    }

    // 3) 現在方向を目標方向へ追従（急に折れない）
    {
        const float t = clamp01(m_TurnResponse * dt);
        m_MoveDir = normalize3(lerp3(m_MoveDir, m_TargetDir, t));
    }

    // 4) 方向に従って速度を決める（=直進だけじゃなくなる）
    {
        m_Velocity.x = m_MoveDir.x * m_Speed;
        m_Velocity.y = m_MoveDir.y * m_Speed;
        m_Velocity.z = m_MoveDir.z * m_Speed;
    }

    // 5) 物理へ適用
    {
        Rigidbody3D rb = GetComponent<Rigidbody3D>();
        if (rb)
        {
            rb->velocity = m_Velocity;
        }
    }

    if (transform->position.y < m_MinAltitude)
    {
        transform->position.y = m_MinAltitude;
        if (m_Velocity.y < 0.0f)
        {
            m_Velocity.y = 0.0f;
            Rigidbody3D rb = GetComponent<Rigidbody3D>();
            if (rb)
            {
                rb->velocity = m_Velocity;
            }
        }
    }

    // 6) 見た目回転：進行方向へ向ける（Transformがある場合）
    {
        auto transform = GetComponent<Transform>();
        if (transform)
        {
            // yaw: atan2(x,z)
            const float yaw = std::atan2(m_MoveDir.x, m_MoveDir.z);
            // pitch: 上下（y）を反映（符号はお好みで）
            const float pitch = -std::atan2(m_MoveDir.y, std::sqrt(m_MoveDir.x * m_MoveDir.x + m_MoveDir.z * m_MoveDir.z));

            transform->degrees.y = rad_to_deg(yaw);
            transform->degrees.x = rad_to_deg(pitch);

            // ロールは任意。ここは「それっぽく」軽く入れるだけ（不要なら0でOK）
            transform->degrees.z = -rad_to_deg(yaw) * 0.4f;
        }
    }
}

void Enemy::UpdateAttack()
{
    if (!m_IsActive || m_IsDying)
    {
        return;
    }

    const float dt = DeltaTime();

    if (m_AttackPattern == AttackPattern::Ram)
    {
        UpdateRam(dt);
        return;
    }

    if (m_IsApproaching)
    {
        return;
    }

    m_AttackTimer -= dt;
    if (m_AttackTimer > 0.0f)
    {
        return;
    }

    if (m_AttackPattern == AttackPattern::Burst)
    {
        FireBurst();
        m_AttackTimer = m_BurstInterval;
    }
    else if (m_AttackPattern == AttackPattern::HomingMissile)
    {
        FireMissile();
        m_AttackTimer = m_MissileInterval;
    }
}

void Enemy::FireBurst()
{
    if (!m_Generator || !m_Player)
    {
        return;
    }

    const math::float3 toPlayer = m_Player->transform->position - transform->position;
    const math::float3 baseDir = normalize3(toPlayer);

    const float offsets[3] = { -m_BurstSpread, 0.0f, m_BurstSpread };
    for (float offset : offsets)
    {
        math::float3 dir = math::float3(baseDir.x + offset, baseDir.y, baseDir.z);
        dir = normalize3(dir);
        m_Generator->GenerateEnemyBullet(transform->position, dir);
    }
}

void Enemy::FireMissile()
{
    if (!m_Generator)
    {
        return;
    }

    m_Generator->GenerateEnemyMissile(transform->position);
}

void Enemy::UpdateRam(float dt)
{
    if (m_IsApproaching)
    {
        return;
    }

    if (!m_IsRamming)
    {
        m_RamPrepareTimer += dt;
        if (m_RamPrepareTimer >= m_RamPrepareDelay)
        {
            m_IsRamming = true;
        }
    }
}

void Enemy::BeginDead()
{
    // 1) 状態更新
    m_IsDying = true;
    m_IsActive = false;      // 通常の Move は止める
    m_DeadTimer = m_DeadDuration;

    // 2) ここで初期の墜落方向・速度を決める
    Rigidbody3D rb = GetComponent<Rigidbody3D>();
    if (rb)
    {
        // 前方＋少し下向きに飛ばす
        math::float3 v = math::float3::Zero();
        v.z = m_Speed * 0.5f;   // 速度は少し遅く
        v.y = -50.0f;           // 下方向に落とす（符号は座標系に合わせて調整）
        rb->velocity = v;
    }

    // 3) コライダー無効化できるならここで無効化（多重ヒット防止）
    // auto collider = GetComponent<Collider3D>();
    // if (collider)
    // {
    //     collider->enabled = false;
    // }
}

void Enemy::DeadAnimation()
{
    // 1) dt
    const float dt = DeltaTime(); // or GetDeltaTime() など、実際のAPIに合わせること

    // 2) タイマー更新
    m_DeadTimer -= dt;

    // 3) 回転させて「墜落感」を出す
    auto transform = GetComponent<Transform>();
    if (transform)
    {
        transform->degrees.x += 2.0f * dt;  // 度前提
        transform->degrees.z += 4.0f * dt;
    }

    // 4) もし Rigidbody に重力をかけたいならここでも追い加速
    Rigidbody3D rb = GetComponent<Rigidbody3D>();
    if (rb)
    {
        math::float3 v = rb->velocity;
        v.y -= 9.8f * dt * 10.0f; // 適当に強めの重力
        rb->velocity = v;
    }

    // 5) 一定時間経過したら削除
    if (m_DeadTimer <= 0.0f)
    {
        DestroyGameObject(&gameObject);
    }
}

REGISTER_SCRIPT_FACTORY(Enemy);
