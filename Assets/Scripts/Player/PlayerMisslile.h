#pragma once
#include "Marionnette.h"

class PlayerMisslile : public Marionnette
{
public:
	// コンストラクタ
    PlayerMisslile(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
};