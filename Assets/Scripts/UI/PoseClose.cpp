#include "PoseClose.h"
#include "uiState.h"
using namespace theatriaSystem;

// Start の処理
void PoseClose::Start()
{
    // 初期化処理
	auto sprite = GetComponent<UISprite>();
    // if の処理
    if (sprite)
    {
        defaultPosX = sprite->position.x;
	}
}

// Update の処理
void PoseClose::Update()
{
    // 毎フレーム処理
    auto mtl = GetComponent<Material>();
	auto sprite = GetComponent<UISprite>();
    // if の処理
    if (mtl && sprite)
    {
        // ここにマテリアルを使った処理を記述
        if ((pose_open))
        {
            mtl->color.a = 1.0f; // ポーズ画面が開いているときは不透明
            // if の処理
            if (currentPoseUISelect == poseUISelect::close)
            {
                // 左右に揺れるアニメーション
                static float time = 0.0f;
                time += DeltaTime();
                float amplitude = 5.0f; // 揺れの振幅
                // 揺れの速さ
                float frequency = 6.0f;
                sprite->position.x = math::Lerp(defaultPosX - amplitude, defaultPosX + amplitude, (sin(time * frequency) + 1.0f) / 2.0f);
                // if の処理
                if(Input::TriggerKey(DIK_SPACE))
                {
                    pose_open = false;
                    currentPoseUISelect = poseUISelect::None;
				}
            }
        }
        else
        {
            mtl->color.a = 0.0f; // ポーズ画面が閉じているときは透明
        }
    }
}

REGISTER_SCRIPT_FACTORY(PoseClose);
