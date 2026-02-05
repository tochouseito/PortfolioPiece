#include "GameOverText.h"
using namespace theatriaSystem;

// Start の処理
void GameOverText::Start()
{
    // 初期化処理
}

// Update の処理
void GameOverText::Update()
{
    // 毎フレーム処理
    FadeIn();
    FadeOut();
}

// StartFadeIn の処理
void GameOverText::StartFadeIn()
{
	// フェードインを開始フラグを立てる
	if (fadingIn || fadingOut) return; // すでにフェード中なら何もしない
	fadingIn = true;
	elapsedTime = 0.0f; // 経過時間をリセット
	// スプライトの透明度を0に設定
	auto mtl = GetComponent<Material>();
	if (mtl)
	{
		mtl->color.a = 0.0f;
	}
}

// StartFadeOut の処理
void GameOverText::StartFadeOut()
{
	// フェードアウトを開始フラグを立てる
	if (fadingOut || fadingIn) return; // すでにフェード中なら何もしない
	fadingOut = true;
	elapsedTime = 0.0f; // 経過時間をリセット
	// スプライトの透明度を1に設定
	auto mtl = GetComponent<Material>();
	if (mtl)
	{
		mtl->color.a = 1.0f;
	}
}

// FadeIn の処理
void GameOverText::FadeIn()
{
	if (!fadingIn) return;
	elapsedTime += DeltaTime();
	auto mtl = GetComponent<Material>();
	if (mtl)
	{
		float alpha = math::Lerp(0.0f, 1.0f, elapsedTime / duration);
		mtl->color.a = alpha;
	}
	if (elapsedTime >= duration)
	{
		// フェードイン完了
		fadingIn = false;
		elapsedTime = duration; // 経過時間を最大にする
	}
}

// FadeOut の処理
void GameOverText::FadeOut()
{
	if (!fadingOut) return;
	elapsedTime += DeltaTime();
	auto mtl = GetComponent<Material>();
	if (mtl)
	{
		float alpha = math::Lerp(1.0f, 0.0f, elapsedTime / duration);
		mtl->color.a = alpha;
	}
	if (elapsedTime >= duration)
	{
		// フェードアウト完了
		fadingOut = false;
		elapsedTime = duration; // 経過時間を最大にする
	}
}

REGISTER_SCRIPT_FACTORY(GameOverText);
