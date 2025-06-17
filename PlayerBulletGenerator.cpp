#include "PlayerBulletGenerator.h"
using namespace ChoSystem;
#include "PlayerBullet.h"

void PlayerBulletGenerator::Start()
{
    // 初期化処理
	m_Player = FindGameObjectByName(L"Player");
	m_PlayerBullet = FindGameObjectByName(L"PlayerBullet");
    
}

void PlayerBulletGenerator::Update()
{
    // 毎フレーム処理
    m_PlayerBullet->transform.scale().Zero();
}

void PlayerBulletGenerator::GenerateBullet(const PlayerBulletType& type, const uint32_t& count)
{
    for(uint32_t i = 0; i < count; ++i)
    {
        // 弾の生成処理
        GameObject* bullet = CloneGameObject(m_PlayerBullet->GetID(), m_Player->transform.position());
        m_PlayerBulletList.push_back(bullet->GetName());
	}
}

REGISTER_SCRIPT_FACTORY(PlayerBulletGenerator);