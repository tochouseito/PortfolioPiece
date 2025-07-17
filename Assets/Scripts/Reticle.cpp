#include "Reticle.h"
#include "Player.h"
#include "Enemy.h"
using namespace ChoSystem;

void Reticle::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
    transform->position = offset;
    transform->parent = m_Player->gameObject.GetHandle().entity;
}

void Reticle::Update()
{
    // 毎フレーム処理
    if (m_Player->transform->position.x < -15.0f)
    {
		transform->parent = std::nullopt; // プレイヤーの親を解除
		Enemy* enemy = GetMarionnette<Enemy>(L"Enemy");
        if (enemy)
        {
			transform->position = enemy->transform->position;
        }
    }
    else
    {
        transform->position = offset;
		transform->parent = m_Player->gameObject.GetHandle().entity;
    }
}

REGISTER_SCRIPT_FACTORY(Reticle);