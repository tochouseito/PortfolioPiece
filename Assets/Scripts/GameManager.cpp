#include "GameManager.h"
using namespace theatriaSystem;
#include "FadeSprite.h"
#include "Player/Player.h"
#include "GameOverText.h"

// Start の処理
void GameManager::Start()
{
    // 初期化処理
	// 各オブジェクト参照を取得
	fadeSprite = GetMarionnette<FadeSprite>(L"FadeSprite");
	player = GetMarionnette<Player>(L"Player");
	gameOverText = GetMarionnette<GameOverText>(L"GameOverText");
	// 起動時にフェードアウト開始
    if (fadeSprite)
    {
        fadeSprite->StartFadeOut();
	}
}

// Update の処理
void GameManager::Update()
{
    // 毎フレーム処理
	// 現状はゲームオーバー/クリア遷移の処理をコメントアウト中
    // 2秒後にフェードイン
    /*if (timer >= 3.0f)
    {
        if (fadeSprite)
        {
            if (!gameOverTriggered)
            {
				gameOverTriggered = true;
                fadeSprite->StartFadeIn();
            }
            if (!fadeSprite->IsFading())
            {
                if (!gameOverStarted)
                {
					gameOverStarted = true;
                    gameOverText->StartFadeIn();
                }
            }
        }
    }
    else
    {
        timer += DeltaTime();
    }
    if (gameOverStarted)
    {
        if (!sceneChanged && Input::TriggerKey(DIK_SPACE))
        {
            sceneChanged = true;
            if (gameOverText && !gameOverText->IsFading())
            {
                gameOverText->StartFadeOut();
            }
        }
        if (sceneChanged && gameOverText && !gameOverText->IsFading())
        {
            sceneManager.LoadScene(L"Title");
            sceneChanged = false;
        }
    }*/

    /*if (timer >= 3.0f)
    {
        if (player)
        {
            player->SetClear();
        }

        if (fadeSprite)
        {
            if (!gameOverTriggered)
            {
                gameOverTriggered = true;
                fadeSprite->StartFadeIn();
            }
            if (!fadeSprite->IsFading())
            {
                if (!gameOverStarted)
                {
                    gameOverStarted = true;
                    gameOverText->StartFadeIn();
                }
            }
        }
    }
    else
    {
        timer += DeltaTime();
    }
    if (gameOverStarted)
    {
        if (!sceneChanged && Input::TriggerKey(DIK_SPACE))
        {
            sceneChanged = true;
            if (gameOverText && !gameOverText->IsFading())
            {
                gameOverText->StartFadeOut();
            }
        }
        if (sceneChanged && gameOverText && !gameOverText->IsFading())
        {
            sceneManager.LoadScene(L"Title");
            sceneChanged = false;
        }
    }*/
}

REGISTER_SCRIPT_FACTORY(GameManager);
