#pragma once
#include "IScript.h"

class BaseCamera : public IScript
{
public:
	// コンストラクタ
    BaseCamera(GameObject& object) : IScript(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
};