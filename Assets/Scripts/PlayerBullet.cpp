#include "PlayerBullet.h"
#include "PlayerBulletGenerator.h" 
using namespace ChoSystem;

void PlayerBullet::Start()
{
    // 初期化処理
    m_Player = &FindGameObjectByName(L"Player");
    gameObject.transform.scale().Initialize();
    m_Direction = ChoMath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), m_Player->transform.quaternion());
	gameObject.transform.quaternion() = m_Player->transform.quaternion();
}

void PlayerBullet::Update()
{
	if (!m_IsActive) return;
    // 毎フレーム処理
    m_Player = &FindGameObjectByName(L"Player");
	
    // テスト
    gameObject.transform.position() += m_Direction.Normalize() * (m_Speed * DeltaTime());
    if(gameObject.transform.position().z > 1000.0f) {
        DestroyGameObject(gameObject.GetID());
        PlayerBulletGenerator* generator = FindGameObjectByName(L"PlayerBulletGenerator").GetScriptInstance<PlayerBulletGenerator>();
        generator->RemoveBulletFromList(gameObject.GetName());
        return;
	}
}

REGISTER_SCRIPT_FACTORY(PlayerBullet);