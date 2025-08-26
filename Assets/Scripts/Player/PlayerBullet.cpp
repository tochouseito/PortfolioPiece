#include "PlayerBullet.h"
using namespace ChoSystem;
#include "Player.h"
#include "Generator/Generator.h"

void PlayerBullet::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
    if (!m_IsActive) { return; }
    m_Direction = chomath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), m_Player->transform->quaternion);
    m_Direction.Normalize();
    m_Velocity.Initialize();
	transform->quaternion = m_Player->transform->quaternion;
	m_LifeTime = 60.0f * 3.0f; // ライフタイムを設定
    // m_Speed = m_Player->GetSpeed() * m_Speed;
    // ハードコーディング！！！
	m_Speed = 40.0f; // 弾の移動速度を設定
}

void PlayerBullet::Update()
{
    if (!m_IsActive) return;

    // テスト
    m_Velocity = m_Direction * m_Speed;
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

    // ライフタイム更新
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