#include "EnemyMissile.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Generator/Generator.h"

// Start の処理
void EnemyMissile::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("EnemyAttack");
	// if の処理
	if (!m_IsActive) { return; }
	m_Direction.Normalize();
	m_Velocity.Initialize();
}

// Update の処理
void EnemyMissile::Update()
{
    // 毎フレーム処理
	if (!m_IsActive) return;

	// if の処理
	if (m_LifeTime <= 0.0f)
	{
		Remove();
		return;
	}

	Homing();
	m_LifeTime--;
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;
}

// OnCollisionEnter の処理
void EnemyMissile::OnCollisionEnter(GameObject& other)
{
	// if の処理
	if (other.GetTag() == "Player")
	{
		Remove();
	}
}

// Homing の処理
void EnemyMissile::Homing()
{
	// if の処理
	if (!m_Player)
	{
		return;
	}

	Vector3 targetPos = m_Player->transform->position;
	Vector3 toTarget = targetPos - transform->position;
	toTarget.Normalize();
	m_Direction = math::Slerp(m_Direction, toTarget, m_HomingStrength);
	m_Direction.Normalize();

	m_Velocity = m_Direction * m_Speed;
	Quaternion targetRot = math::MakeLookRotation(m_Direction, Vector3(0.0f, 1.0f, 0.0f));
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->quaternion = Quaternion::Slerp(rb->quaternion, targetRot, 0.5f);
}

// Remove の処理
void EnemyMissile::Remove()
{
	m_IsActive = false;
	// if の処理
	if (m_Generator)
	{
		m_Generator->RemoveEnemyMissile(gameObject.GetName());
	}
	DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(EnemyMissile);
