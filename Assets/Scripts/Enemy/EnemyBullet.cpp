#include "EnemyBullet.h"
using namespace theatriaSystem;
#include "Generator/Generator.h"

// Start の処理
void EnemyBullet::Start()
{
    // 初期化処理
	// 生成器参照と初期タグ設定
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("EnemyAttack");
	// 非アクティブなら初期化を省略
	if (!m_IsActive) { return; }
	// 方向を正規化し速度ベクトルを初期化
	m_Direction.Normalize();
	m_Velocity.Initialize();
}

// Update の処理
void EnemyBullet::Update()
{
    // 毎フレーム処理
	// 非アクティブなら処理しない
	if (!m_IsActive) return;

	// 寿命切れで削除
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

	// 直進移動と寿命更新
	m_LifeTime--;
	m_Velocity = (m_Direction * m_Speed) * DeltaTime();
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;
}

// OnCollisionEnter の処理
void EnemyBullet::OnCollisionEnter(GameObject& other)
{
	// プレイヤーに当たったら消滅
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
