#pragma once
#include "IScript.h"

class Player : public IScript
{
public:
	// コンストラクタ
    Player(GameObject& object) : IScript(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
    void Move();
    void Boost();
	void SlowDown();

    // ターゲット
	GameObject* target = nullptr;
    // カメラ
	GameObject* camera = nullptr;

    // パラメータ
    float speed = 10.0f; // 移動速度
    float rotateSpeed = 8.0f; // 回転速度
    const float boostPower = 20.0f; // ブースト速度
};