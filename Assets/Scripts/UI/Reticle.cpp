#include "Reticle.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Enemy/Enemy.h"

// Start の処理
void Reticle::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
    transform->position = offset;
    transform->parent = m_Player->gameObject.GetHandle().entity;
}

// Update の処理
void Reticle::Update()
{
    // 毎フレーム処理
}

REGISTER_SCRIPT_FACTORY(Reticle);
