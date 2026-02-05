#include "EnemyBullet.h"
using namespace theatriaSystem;
#include "Generator/Generator.h"

// Start の処理
void EnemyBullet::Start()
{
    // 初期化処理
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("EnemyAttack");
	// if の処理
	if (!m_IsActive) { return; }
	m_Direction.Normalize();
	m_Velocity.Initialize();
}

// Update の処理
void EnemyBullet::Update()
{
    // 毎フレーム処理
	if (!m_IsActive) return;

	// if の処理
	if (m_LifeTime <= 0.0f)
	{
		m_IsActive = false;
		// if の処理
		if (m_Generator)
		{
			m_Generator->RemoveEnemyBullet(gameObject.GetName());
		}
		DestroyGameObject(&gameObject);
		return;
	}

	m_LifeTime--;
	m_Velocity = (m_Direction * m_Speed) * DeltaTime();
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;
}

// OnCollisionEnter の処理
void EnemyBullet::OnCollisionEnter(GameObject& other)
{
	// if の処理
	if (other.GetTag() == "Player")
	{
		m_IsActive = false;
		// if の処理
		if (m_Generator)
		{
			m_Generator->RemoveEnemyBullet(gameObject.GetName());
		}
		DestroyGameObject(&gameObject);
	}
}

REGISTER_SCRIPT_FACTORY(EnemyBullet);
