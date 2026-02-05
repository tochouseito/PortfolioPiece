#pragma once
#include <Marionnette.h>

class FadeSprite;

// タイトル画面の進行と遷移を管理するクラス
class TitleManager : public Marionnette
{
public:
	// コンストラクタ
    TitleManager(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
    FadeSprite* fadeSprite = nullptr;
	bool sceneChanged = false;
};
