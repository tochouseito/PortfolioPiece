#include "Target.h"
using namespace ChoSystem;

void Target::Start()
{
    // 初期化処理
    gameObject.transform.position() = offset;
    gameObject.transform.SetParent(player->GetEntity());
}

void Target::Update()
{
    // 毎フレーム処理
}

REGISTER_SCRIPT_FACTORY(Target);