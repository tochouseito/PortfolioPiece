#include "PlayerBullet.h"
#include "PlayerBulletGenerator.h" 
using namespace ChoSystem;
#include "Player.h"

void PlayerBullet::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
    transform->scale.Initialize();
    m_Direction = chomath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), m_Player->transform->quaternion);
	transform->quaternion = m_Player->transform->quaternion;
}

void PlayerBullet::Update()
{
	if (!m_IsActive) return;
	
    // テスト
    transform->position += m_Direction.Normalize() * (m_Speed * DeltaTime());
    if(transform->position.z > 1000.0f) {
        //DestroyGameObject(gameObject.GetID());
		PlayerBulletGenerator* generator = GetMarionnette<PlayerBulletGenerator>(L"PlayerBulletGenerator");
        generator->RemoveBulletFromList(gameObject.GetName());
        return;
	}
}

REGISTER_SCRIPT_FACTORY(PlayerBullet);