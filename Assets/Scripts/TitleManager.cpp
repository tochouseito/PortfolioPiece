#include "TitleManager.h"
using namespace theatriaSystem;
#include "FadeSprite.h"

void TitleManager::Start()
{
    // 初期化処理
	fadeSprite = GetMarionnette<FadeSprite>(L"FadeSprite");
}

void TitleManager::Update()
{
    // 毎フレーム処理
	// スペースキーが押されたらフェードしてシーン切り替え
	if (!sceneChanged && Input::TriggerKey(DIK_SPACE))
	{
		sceneChanged = true;
		if(fadeSprite && !fadeSprite->IsFading())
		{
			fadeSprite->StartFadeIn();
		}
	}
	if(sceneChanged && fadeSprite && !fadeSprite->IsFading())
	{
		sceneManager.LoadScene(L"MainScene");
		sceneChanged = false;
	}
}

REGISTER_SCRIPT_FACTORY(TitleManager);
