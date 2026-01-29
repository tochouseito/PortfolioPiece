#include "EnemyBullet.h"
using namespace theatriaSystem;
#include "Generator/Generator.h"

void EnemyBullet::Start()
{
    // 初期化処理
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("EnemyAttack");
	if (!m_IsActive) { return; }
	m_Direction.Normalize();
	m_Velocity.Initialize();
}

void EnemyBullet::Update()
{
    // 毎フレーム処理
	if (!m_IsActive) return;

	if (m_LifeTime <= 0.0f)
	{
		m_IsActive = false;
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

void EnemyBullet::OnCollisionEnter(GameObject& other)
{
	if (other.GetTag() == "Player")
	{
		m_IsActive = false;
		if (m_Generator)
		{
			m_Generator->RemoveEnemyBullet(gameObject.GetName());
		}
		DestroyGameObject(&gameObject);
	}
}

REGISTER_SCRIPT_FACTORY(EnemyBullet);
