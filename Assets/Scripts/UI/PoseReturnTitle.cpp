#include "PoseReturnTitle.h"
#include "uiState.h"
using namespace theatriaSystem;

void PoseReturnTitle::Start()
{
    // 初期化処理
	auto sprite = GetComponent<UISprite>();
    if (sprite)
    {
        defaultPosX = sprite->position.x;
	}
}

void PoseReturnTitle::Update()
{
    // 毎フレーム処理
    auto mtl = GetComponent<Material>();
    if (mtl)
    {
        // ここにマテリアルを使った処理を記述
        if ((pose_open))
        {
            mtl->color.a = 1.0f; // ポーズ画面が開いているときは不透明
            if (currentPoseUISelect == poseUISelect::returnTitle)
            {
                // 左右に揺れるアニメーション
                static float time = 0.0f;
                time += DeltaTime();
                float amplitude = 5.0f; // 揺れの振幅
                // 揺れの速さ
                float frequency = 6.0f;
                auto sprite = GetComponent<UISprite>();
                if (sprite)
                {
                    sprite->position.x = math::Lerp(defaultPosX - amplitude, defaultPosX + amplitude, (sin(time * frequency) + 1.0f) / 2.0f);
                }
			}
        }
        else
        {
            mtl->color.a = 0.0f; // ポーズ画面が閉じているときは透明
        }
    }
}

REGISTER_SCRIPT_FACTORY(PoseReturnTitle);
