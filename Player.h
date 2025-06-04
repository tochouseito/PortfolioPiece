#pragma once
#include "IScript.h"

class PlayerBulletGenerator; // 前方宣言
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
    void Attack();

    // ターゲット
	GameObject* m_Target = nullptr;
    // カメラ
	GameObject* m_Camera = nullptr;
    // 弾生成器
	GameObject* m_BulletGenerator = nullptr;
	PlayerBulletGenerator* m_BulletGeneratorScript = nullptr;

    // パラメータ
    float speed = 10.0f; // 移動速度
    float rotateSpeed = 8.0f; // 回転速度
    const float boostPower = 20.0f; // ブースト速度
};