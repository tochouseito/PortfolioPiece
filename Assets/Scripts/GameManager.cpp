#include "GameManager.h"
using namespace theatriaSystem;
#include "FadeSprite.h"

void GameManager::Start()
{
    // 初期化処理
	fadeSprite = GetMarionnette<FadeSprite>(L"FadeSprite");
    if(fadeSprite)
    {
        fadeSprite->StartFadeOut();
	}
}

void GameManager::Update()
{
    // 毎フレーム処理
}

REGISTER_SCRIPT_FACTORY(GameManager);
