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

	// 回避行動フラグGet
	bool IsDodging() const { return isDodging; }
private:
    void Move();
    void Boost();
	void SlowDown();
    void Attack();
    void Dodge();

    // ターゲット
	GameObject* m_Target = nullptr;
    // カメラ
	GameObject* m_Camera = nullptr;
    // 弾生成器
	GameObject* m_BulletGenerator = nullptr;
	PlayerBulletGenerator* m_BulletGeneratorScript = nullptr;

    // パラメータ
	Vector3 velocity;// 現在の速度
    float speed = 10.0f; // 移動速度
    float rotateSpeed = 50.0f; // 回転速度
    const float boostPower = 20.0f; // ブースト速度
    float dodgeMoveSpeed = 1.5f;      // 回避の移動速度
    bool isDodging = false;
    float dodgeTimer = 0.0f;
    int dodgeDirection = 0; // -1 = 左, 1 = 右
    const float dodgeDuration = 0.3f;
    float dodgeRotateSpeed = 360.0f; // 1秒間に1回転（度）
};