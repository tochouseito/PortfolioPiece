#include "Target.h"
using namespace ChoSystem;
#include "Player/Player.h"

void Target::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
    transform->position = offset;
    transform->parent = m_Player->gameObject.GetHandle().entity;
}

void Target::Update()
{
    // 毎フレーム処理
}

REGISTER_SCRIPT_FACTORY(Target);