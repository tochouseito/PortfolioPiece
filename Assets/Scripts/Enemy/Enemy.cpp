#include "Enemy.h"
using namespace theatriaSystem;
#include "Enemy/EnemySpawner.h"
#include "UI/Target.h"
#include "UI/LockOn.h"

#include <cmath>
#include <algorithm>

namespace
{
    constexpr float kPi = 3.14159265358979323846f;

    float clamp01(float v)
    {
        return std::clamp(v, 0.0f, 1.0f);
    }

    float deg_to_rad(float deg)
    {
        return deg * (kPi / 180.0f);
    }

    float rad_to_deg(float rad)
    {
        return rad * (180.0f / kPi);
    }

    float length3(const math::float3& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    math::float3 normalize3(const math::float3& v)
    {
        const float len = length3(v);
        if (len <= 1e-6f)
        {
            return math::float3(0.0f, 0.0f, 1.0f);
        }
        return math::float3(v.x / len, v.y / len, v.z / len);
    }

    math::float3 lerp3(const math::float3& a, const math::float3& b, float t)
    {
        return math::float3(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t);
    }
}

void Enemy::Start()
{
    // 1) 参照取得
    m_EnemySpawner = GetMarionnette<EnemySpawner>(L"EnemySpawner");
    m_Target = GetMarionnette<Target>(L"Target");

    // 2) タグ設定
    gameObject.SetTag("Enemy");

    // 3) 非アクティブなら何もしない
    if (!m_IsActive)
    {
        return;
    }
}

void Enemy::Update()
{
    // 1) 死亡アニメ優先
    if (m_IsDying)
    {
        DeadAnimation();
        return;
    }

    // 2) 非アクティブは更新しない
    if (!m_IsActive)
    {
        return;
    }

    // 3) 移動
    Move();
}

void Enemy::EnableLockOnTarget(LockOn* lockOn)
{
    m_IsLockOnTarget = true;
    m_LockOn = lockOn;
}

void Enemy::UnableLockOnTarget()
{
    if (!m_IsLockOnTarget)
    {
        return;
    }

    m_IsLockOnTarget = false;

    // 1) LockOn UI 側から登録解除
    m_Target->RemoveLockOn(m_LockOn->gameObject.GetName());

    // 2) LockOn UI 自体を破棄
    DestroyGameObject(&m_LockOn->gameObject);
    m_LockOn = nullptr;
}

void Enemy::OnCollisionEnter(GameObject& other)
{
    if (other.GetTag() != "PlayerAttack")
    {
        return;
    }

    if (m_IsDying)
    {
        return;
    }

    // 1) スポーン管理から除外
    if (m_EnemySpawner)
    {
        m_EnemySpawner->RemoveEnemy(gameObject.GetName());
    }

    // 2) ロックオン解除
    UnableLockOnTarget();

    // 3) 死亡開始
    BeginDead();
}

void Enemy::Move()
{
    // 1) dt
    const float dt = DeltaTime();

    // 2) パターンから「望ましい進行方向」を計算
    const math::float3 desiredDir = CalcDesiredDir(dt);

    // 3) 現在dirを望ましいdirへ追従（急旋回しすぎないように）
    const float t = clamp01(m_MoveSettings.turnResponse * dt);
    m_MoveDir = normalize3(lerp3(m_MoveDir, desiredDir, t));

    // 4) 速度・見た目回転に適用
    ApplyMove(m_MoveDir);
}

math::float3 Enemy::CalcDesiredDir(float dt)
{
    // 1) 経過時間
    m_MoveTime += dt;

    // 2) 基本は前（ワールドZ前提）
    math::float3 dir = math::float3(0.0f, 0.0f, 1.0f);

    // 3) パターン分岐
    switch (m_MovePattern)
    {
    case MovePattern::Straight:
    {
        // 直進：何もしない
        break;
    }
    case MovePattern::WeaveYaw:
    {
        // S字：ヨー角を正弦波で振る
        const float w = 2.0f * kPi * m_MoveSettings.weaveFreqHz;
        const float yawRad = std::sin(w * m_MoveTime) * deg_to_rad(m_MoveSettings.weaveYawDeg);

        dir.x = std::sin(yawRad);
        dir.z = std::cos(yawRad);
        dir.y = 0.0f;
        break;
    }
    case MovePattern::SwoopPitch:
    {
        // 上下：ピッチ角を正弦波で振る（上昇→下降）
        const float w = 2.0f * kPi * m_MoveSettings.swoopFreqHz;
        const float pitchRad = std::sin(w * m_MoveTime) * deg_to_rad(m_MoveSettings.swoopPitchDeg);

        // ピッチは y 成分に反映（簡易）
        dir.y = std::sin(pitchRad);
        dir.z = std::cos(pitchRad);
        dir.x = 0.0f;
        break;
    }
    case MovePattern::ConstantTurn:
    {
        // 一定旋回：時間に比例してヨーを回す
        const float yawRad = deg_to_rad(m_MoveSettings.constTurnDegPerSec) * m_MoveTime;

        dir.x = std::sin(yawRad);
        dir.z = std::cos(yawRad);
        dir.y = 0.0f;
        break;
    }
    default:
    {
        break;
    }
    }

    // 4) 正規化して返す
    return normalize3(dir);
}

void Enemy::ApplyMove(const math::float3& dir)
{
    // 1) 速度を設定（飛行は「前進速度×dir」で十分）
    m_Velocity = math::float3(dir.x * m_Speed, dir.y * m_Speed, dir.z * m_Speed);

    Rigidbody3D rb = GetComponent<Rigidbody3D>();
    if (rb)
    {
        rb->velocity = m_Velocity;
    }

    // 2) 見た目：向き合わせ＋バンク（Transformがある前提）
    auto transform = GetComponent<Transform>();
    if (transform)
    {
        // yaw/pitch 推定（x右, y上, z前）
        const float yaw = std::atan2(dir.x, dir.z);
        const float pitch = -std::atan2(dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));

        transform->degrees.y = rad_to_deg(yaw);
        transform->degrees.x = rad_to_deg(pitch);

        // バンク：旋回してる雰囲気だけ作る（雑でいい。アケゲーはこれで成立する）
        transform->degrees.z = -rad_to_deg(yaw) * m_MoveSettings.bankFactor;
    }
}

// ---- 以降 BeginDead / DeadAnimation はあなたのままでOK（必要なら後で統合） ----


void Enemy::BeginDead()
{
	m_IsDying = true;
	m_IsActive = false;      // 通常の Move は止める
	m_DeadTimer = m_DeadDuration;

	// ここで初期の墜落方向・速度を決める
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	if (rb)
	{
		// 前方＋少し下向きに飛ばす
		math::float3 v = math::float3::Zero();
		v.z = m_Speed * 0.5f;   // 速度は少し遅く
		v.y = -50.0f;           // 下方向に落とす（符号はエンジンの座標系に合わせて調整）
		rb->velocity = v;
	}

	// コライダーを無効化できるならここで無効化する（多重ヒット防止）
	// auto collider = GetComponent<Collider3D>();
	// if (collider) { collider->enabled = false; }
}

void Enemy::DeadAnimation()
{
	// Time::DeltaTime() 的なものがエンジンにある前提
	const float dt = DeltaTime(); // or GetDeltaTime() など、実際のAPIに合わせること

	m_DeadTimer -= dt;

	// 回転させて「墜落感」を出す
	auto transform = GetComponent<Transform>();
	if (transform)
	{
		// ピッチ方向に機首を下げながら、ロールさせる
		transform->degrees.x += 2.0f * dt;  // ラジアン前提なら小さめ、度なら調整
		transform->degrees.z += 4.0f * dt;
	}

	// もし Rigidbody に重力をかけたいならここでも追い加速
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	if (rb)
	{
		math::float3 v = rb->velocity;
		v.y -= 9.8f * dt * 10.0f; // 適当に強めの重力
		rb->velocity = v;
	}

	// 一定時間経過したらオブジェクトを削除
	if (m_DeadTimer <= 0.0f)
	{
		DestroyGameObject(&gameObject);
	}
}

REGISTER_SCRIPT_FACTORY(Enemy);