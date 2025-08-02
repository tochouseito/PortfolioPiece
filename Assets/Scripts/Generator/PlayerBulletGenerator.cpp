#include "PlayerBulletGenerator.h"
using namespace ChoSystem;
#include "Player.h"
#include "PlayerBullet.h"

void PlayerBulletGenerator::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
	m_PlayerBullet = GetMarionnette<PlayerBullet>(L"PlayerBullet");

    if (m_PlayerBullet)
    {
        m_PlayerBullet->transform->scale.Zero();
    }
}

void PlayerBulletGenerator::Update()
{
    // 毎フレーム処理
	m_PlayerBullet->transform->scale.Zero();
    for(auto& bulletName : m_PlayerBulletList)
    {
        GameObject* bullet = FindGameObjectByName(bulletName);
        PlayerBullet* playerBullet = bullet->GetScriptInstance<PlayerBullet>();
        if (playerBullet && !playerBullet->IsActive())
        {
			playerBullet->SetActive(true);
        }
	}
}

void PlayerBulletGenerator::GenerateBullet(const PlayerBulletType& type, const uint32_t& count)
{
    for(uint32_t i = 0; i < count; ++i)
    {
        // 弾の生成処理
        GameObject* bullet = CloneGameObject(&m_PlayerBullet->gameObject, m_Player->transform->position);
        
        m_PlayerBulletList.push_back(bullet->GetName());
	}
}

void PlayerBulletGenerator::RemoveBulletFromList(const std::wstring& bulletName)
{
	m_PlayerBulletList.remove(bulletName);
}

REGISTER_SCRIPT_FACTORY(PlayerBulletGenerator);