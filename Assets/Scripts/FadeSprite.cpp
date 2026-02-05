#include "FadeSprite.h"
using namespace theatriaSystem;

void FadeSprite::Start()
{
    // 初期化処理
    REGISTER_FIELD_MINMAX(duration, 0.0f, 100.0f);
	elapsedTime = 0.0f;
}

void FadeSprite::Update()
{
    // 毎フレーム処理
    FadeIn();
    FadeOut();
}

void FadeSprite::StartFadeIn()
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

void FadeSprite::StartFadeOut()
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

void FadeSprite::FadeIn()
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

void FadeSprite::FadeOut()
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

REGISTER_SCRIPT_FACTORY(FadeSprite);
