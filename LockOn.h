#pragma once
#include "Marionnette.h"

class LockOn : public Marionnette
{
public:
	// コンストラクタ
    LockOn(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
};