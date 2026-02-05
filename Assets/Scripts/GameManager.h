#pragma once
#include <Marionnette.h>

class FadeSprite;
class Player;
class GameOverText;

// ゲーム進行（開始/終了/クリア）を管理するクラス
class GameManager : public Marionnette
{
public:
	// コンストラクタ
	GameManager(GameObject& object) : Marionnette(object) {}
	// 初期化処理
	void Start() override;
	// 毎フレーム処理
	void Update() override;
private:
	FadeSprite* fadeSprite = nullptr;
	Player* player = nullptr;
	GameOverText* gameOverText = nullptr;
	bool started = false;
	float timer = 0.0f;
	bool gameOverTriggered = false;
	bool gameOverStarted = false;
	bool sceneChanged = false;

	bool clearTriggered = false;
	bool clearStarted = false;
};
