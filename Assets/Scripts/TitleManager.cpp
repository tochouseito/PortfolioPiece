#include "TitleManager.h"
using namespace theatriaSystem;
#include "FadeSprite.h"

// Start の処理
void TitleManager::Start()
{
    // 初期化処理
	fadeSprite = GetMarionnette<FadeSprite>(L"FadeSprite");
}

// Update の処理
void TitleManager::Update()
{
    // 毎フレーム処理
	// スペースキーが押されたらフェードしてシーン切り替え
	if (!sceneChanged && Input::TriggerKey(DIK_SPACE))
	{
		sceneChanged = true;
		// if の処理
		if(fadeSprite && !fadeSprite->IsFading())
		{
			fadeSprite->StartFadeIn();
		}
	}
	// if の処理
	if(sceneChanged && fadeSprite && !fadeSprite->IsFading())
	{
		sceneManager.LoadScene(L"MainScene");
		sceneChanged = false;
	}
}

REGISTER_SCRIPT_FACTORY(TitleManager);
