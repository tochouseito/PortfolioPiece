#include "PoseBackend.h"
#include "uiState.h"
using namespace theatriaSystem;

// Start の処理
void PoseBackend::Start()
{
    // 初期化処理
}

// Update の処理
void PoseBackend::Update()
{
    // 毎フレーム処理
    // ESCでポーズ開閉をトグル
    if (Input::TriggerKey(DIK_ESCAPE))
    {
        pose_open = !pose_open;
    }
	// 背景の透明度と選択状態を更新
	auto mtl = GetComponent<Material>();
    if (mtl)
    {
        // ここにマテリアルを使った処理を記述
        if((pose_open))
        {
            mtl->color.a = 0.5f; // ポーズ画面が開いているときは不透明
            // 上下入力で選択を切り替え
            if(Input::TriggerKey(DIK_W))
            {
                currentPoseUISelect = poseUISelect::returnTitle;
            }
            else if(Input::TriggerKey(DIK_S))
            {
                currentPoseUISelect = poseUISelect::close;
			}
        }
        else
        {
            mtl->color.a = 0.0f; // ポーズ画面が閉じているときは透明
		}
	}
}

REGISTER_SCRIPT_FACTORY(PoseBackend);
