#include "PlayerMissile.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Generator/Generator.h"

void PlayerMissile::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	// タグ設定
	gameObject.SetTag("PlayerAttack");
	if (!m_IsActive) { return; }
    m_Direction.Normalize();
    m_Velocity.Initialize();
    m_Angle = transform->degrees;
    transform->quaternion = m_Player->transform->quaternion;
	m_LifeTime = 60.0f * 6.0f; // ライフタイムを設定
}

void PlayerMissile::Update()
{
    // 毎フレーム処理
	if (!m_IsActive) return;
	// ライフタイムが0以下なら消滅
    if (m_LifeTime <= 0.0f)
    {
        // ライフタイムが0以下なら非アクティブにする
        Remove();
    }
	// ホーミング処理
	Homing();
    // ライフタイム更新
    m_LifeTime--;
    Rigidbody3D rb = GetComponent<Rigidbody3D>();
    rb->velocity = m_Velocity; // Rigidbody3Dの速度を更新
    rb.SetTransform(rb->quaternion);
}

// 衝突処理
void PlayerMissile::OnCollisionEnter(GameObject& other)
{
    // otherがEnemyなら
    if (other.GetTag() == "Enemy")
    {
        Remove();
    }
}

void PlayerMissile::Homing()
{
    Rigidbody3D rb = GetComponent<Rigidbody3D>();

    // 移動
	Vector3 enemyPos = m_TargetEnemy->transform->position;
	Vector3 toEnemy = enemyPos - transform->position;
	toEnemy.Normalize();
    m_Direction = toEnemy;
    m_Velocity = chomath::Slerp(m_Velocity, m_Direction, 1.0f);
    m_Velocity.Normalize();

    // 速度イージング
    // 経過時間割合
    float t = m_ElapsedTime / m_AccelDuration;
	t = chomath::Clamp(t, 0.0f, 1.0f);
	// イージング関数で補正（EaseOutQuad）
	float ease = chomath::easing::EaseInQuad(t);
	// 速度に反映
	m_CurrentSpeed = chomath::Lerp(m_Player->GetSpeed(), m_MaxSpeed, ease);
    
    m_ElapsedTime += DeltaTime();

    m_Velocity *= m_CurrentSpeed;

    // 回転姿勢をターゲット方向に向ける
    Quaternion targetRot = chomath::MakeLookRotation(m_Velocity, Vector3(0.0f,1.0f,0.0f));
    // 現在の回転から目標回転へ補間
	rb->quaternion = Quaternion::Slerp(rb->quaternion, targetRot, 1.0f);
}

void PlayerMissile::Remove()
{
	// ミサイルを非アクティブにする
    m_IsActive = false;
    // 生成器から削除
    if (m_Generator)
    {
        m_Generator->RemovePlayerMissile(gameObject.GetName());
    }
    DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(PlayerMissile);