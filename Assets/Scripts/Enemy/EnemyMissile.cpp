#include "EnemyMissile.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Generator/Generator.h"

void EnemyMissile::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("EnemyAttack");
	if (!m_IsActive) { return; }
	m_Direction.Normalize();
	m_Velocity.Initialize();
}

void EnemyMissile::Update()
{
    // 毎フレーム処理
	if (!m_IsActive) return;

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

void EnemyMissile::OnCollisionEnter(GameObject& other)
{
	if (other.GetTag() == "Player")
	{
		Remove();
	}
}

void EnemyMissile::Homing()
{
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

void EnemyMissile::Remove()
{
	m_IsActive = false;
	if (m_Generator)
	{
		m_Generator->RemoveEnemyMissile(gameObject.GetName());
	}
	DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(EnemyMissile);
