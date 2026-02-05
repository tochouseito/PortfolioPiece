#pragma once
#include "Marionnette.h"

// 画面フェード用スプライトを制御するクラス
class FadeSprite : public Marionnette
{
public:
	// コンストラクタ
    FadeSprite(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// フェードインを開始する
	void StartFadeIn();
	// フェードアウトを開始する
	void StartFadeOut();
	// フェードイン処理
	void FadeIn();;
	// フェードアウト処理
	void FadeOut();
	// フェード中かどうか
	bool IsFading() const { return fadingIn || fadingOut; }
private:
	float duration = 1.0f; // フェードの持続時間
	float elapsedTime = 0.0f; // 経過時間
	bool fadingIn = false; // フェードイン中かどうか
	bool fadingOut = false; // フェードアウト中かどうか
};
