#include "EnemyMissile.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Generator/Generator.h"

// Start の処理
void EnemyMissile::Start()
{
    // 初期化処理
	// 参照取得とタグ設定
	m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("EnemyAttack");
	// 非アクティブなら初期化を省略
	if (!m_IsActive) { return; }
	// 方向と速度ベクトルを初期化
	m_Direction.Normalize();
	m_Velocity.Initialize();
}

// Update の処理
void EnemyMissile::Update()
{
    // 毎フレーム処理
	// 非アクティブなら処理しない
	if (!m_IsActive) return;

	// 寿命切れで削除
	if (m_LifeTime <= 0.0f)
	{
		Remove();
		return;
	}

	// ホーミング更新と移動
	Homing();
	m_LifeTime--;
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;
}

// OnCollisionEnter の処理
void EnemyMissile::OnCollisionEnter(GameObject& other)
{
	// プレイヤーに当たったら消滅
	if (other.GetTag() == "Player")
	{
		Remove();
	}
}

// Homing の処理
void EnemyMissile::Homing()
{
	// プレイヤー参照が無ければ追尾できない
	if (!m_Player)
	{
		return;
	}

	// プレイヤー方向へ徐々に向きを変える
	Vector3 targetPos = m_Player->transform->position;
	Vector3 toTarget = targetPos - transform->position;
	toTarget.Normalize();
	m_Direction = math::Slerp(m_Direction, toTarget, m_HomingStrength);
	m_Direction.Normalize();

	// 速度と見た目回転を反映
	m_Velocity = m_Direction * m_Speed;
	Quaternion targetRot = math::MakeLookRotation(m_Direction, Vector3(0.0f, 1.0f, 0.0f));
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->quaternion = Quaternion::Slerp(rb->quaternion, targetRot, 0.5f);
}

// Remove の処理
void EnemyMissile::Remove()
{
	// 生成器から登録解除して破棄
	m_IsActive = false;
	if (m_Generator)
	{
		m_Generator->RemoveEnemyMissile(gameObject.GetName());
	}
	DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(EnemyMissile);
