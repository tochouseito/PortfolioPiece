#include "GameManager.h"
using namespace theatriaSystem;
#include "FadeSprite.h"
#include "Player/Player.h"
#include "GameOverText.h"

// Start の処理
void GameManager::Start()
{
    // 初期化処理
	fadeSprite = GetMarionnette<FadeSprite>(L"FadeSprite");
	player = GetMarionnette<Player>(L"Player");
	gameOverText = GetMarionnette<GameOverText>(L"GameOverText");
    // if の処理
    if(fadeSprite)
    {
        fadeSprite->StartFadeOut();
	}
}

// Update の処理
void GameManager::Update()
{
    // 毎フレーム処理
    // 2秒後にフェードイン
    /*if (timer >= 3.0f)
    {
        // if の処理
        if (fadeSprite)
        {
            // if の処理
            if (!gameOverTriggered)
            {
				gameOverTriggered = true;
                fadeSprite->StartFadeIn();
            }
            // if の処理
            if (!fadeSprite->IsFading())
            {
                // if の処理
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
    // if の処理
    if (gameOverStarted)
    {
        // if の処理
        if (!sceneChanged && Input::TriggerKey(DIK_SPACE))
        {
            sceneChanged = true;
            // if の処理
            if (gameOverText && !gameOverText->IsFading())
            {
                gameOverText->StartFadeOut();
            }
        }
        // if の処理
        if (sceneChanged && gameOverText && !gameOverText->IsFading())
        {
            sceneManager.LoadScene(L"Title");
            sceneChanged = false;
        }
    }*/

    /*if (timer >= 3.0f)
    {
        // if の処理
        if (player)
        {
            player->SetClear();
        }

        // if の処理
        if (fadeSprite)
        {
            // if の処理
            if (!gameOverTriggered)
            {
                gameOverTriggered = true;
                fadeSprite->StartFadeIn();
            }
            // if の処理
            if (!fadeSprite->IsFading())
            {
                // if の処理
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
    // if の処理
    if (gameOverStarted)
    {
        // if の処理
        if (!sceneChanged && Input::TriggerKey(DIK_SPACE))
        {
            sceneChanged = true;
            // if の処理
            if (gameOverText && !gameOverText->IsFading())
            {
                gameOverText->StartFadeOut();
            }
        }
        // if の処理
        if (sceneChanged && gameOverText && !gameOverText->IsFading())
        {
            sceneManager.LoadScene(L"Title");
            sceneChanged = false;
        }
    }*/
}

REGISTER_SCRIPT_FACTORY(GameManager);
