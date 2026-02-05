#pragma once
#include "Marionnette.h"

// ポーズ画面の背景UIクラス
class PoseBackend : public Marionnette
{
public:
	// コンストラクタ
    PoseBackend(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
};
