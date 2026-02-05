#include "PlayerBullet.h"
using namespace theatriaSystem;
#include "Player.h"
#include "Generator/Generator.h"

// Start の処理
void PlayerBullet::Start()
{
    // 初期化処理
	// 参照取得とタグ設定
    m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	// タグ設定
	gameObject.SetTag("PlayerAttack");
	// 非アクティブなら初期化を省略
    if (!m_IsActive) { return; }
	// プレイヤーの向きを基準に進行方向を決定
    m_Direction = math::RotateVector(math::float3(0.0f, 0.0f, 1.0f), m_Player->transform->quaternion);
    m_Direction.Normalize();
    m_Velocity.Initialize();
	transform->quaternion = m_Player->transform->quaternion;
	m_LifeTime = 60.0f * 3.0f; // ライフタイムを設定
    // m_Speed = m_Player->GetSpeed() * m_Speed;
}

// Update の処理
void PlayerBullet::Update()
{
	// 非アクティブなら処理しない
    if (!m_IsActive) return;

    // テスト
	m_Velocity = (m_Direction * m_Speed) * DeltaTime();
	// 寿命切れで削除
    if (m_LifeTime <= 0.0f)
    {
        // ライフタイムが0以下なら非アクティブにする
        m_IsActive = false;
        // 生成器から削除
        if (m_Generator)
        {
            m_Generator->RemovePlayerBullet(gameObject.GetName());
        }
		DestroyGameObject(&gameObject);
    }

    // ライフタイム更新と物理速度反映
	m_LifeTime--;
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity; // Rigidbody3Dの速度を更新
}

// 衝突時の処理
void PlayerBullet::OnCollisionEnter(GameObject& other) 
{
    // otherがEnemyなら
    if (other.GetTag() == "Enemy")
    {
        // 弾を非アクティブにする
        m_IsActive = false;
        // 生成器から削除
        if (m_Generator) {
            m_Generator->RemovePlayerBullet(gameObject.GetName());
        }
		DestroyGameObject(&gameObject);
    }
}

REGISTER_SCRIPT_FACTORY(PlayerBullet);
