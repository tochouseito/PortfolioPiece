#include "Target.h"
using namespace ChoSystem;

void Target::Start()
{
    // 初期化処理
    player = FindGameObjectByName(L"Player");
    gameObject.transform.position() = offset;
    gameObject.transform.SetParent(player->GetEntity());
}

void Target::Update()
{
    // 毎フレーム処理
    //gameObject.transform.position() = player->transform.position();
    //gameObject.transform.position() += player->transform.forward() * offset;
}

REGISTER_SCRIPT_FACTORY(Target);