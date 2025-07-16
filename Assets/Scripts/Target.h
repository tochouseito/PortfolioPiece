#pragma once
#include "Marionnette.h"

class Target : public Marionnette
{
public:
	// コンストラクタ
    Target(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
    // プレイヤー
	GameObject* player = nullptr;

    // パラメータ
    Vector3 offset = Vector3(0.0f, 0.0f, 40.0f);
};