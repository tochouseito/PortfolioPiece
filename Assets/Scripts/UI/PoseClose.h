#pragma once
#include "Marionnette.h"

class PoseClose : public Marionnette
{
public:
	// コンストラクタ
    PoseClose(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
	float defaultPosX = 0.0f;
};