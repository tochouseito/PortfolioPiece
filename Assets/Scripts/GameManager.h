#pragma once
#include "Marionnette.h"

class FadeSprite;
class Player;
class GameOverText;

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
};