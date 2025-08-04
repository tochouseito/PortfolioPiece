#pragma once
#include "Marionnette.h"

// 前方宣言
class Target;
class MainCamera;
class Generator;

class Player : public Marionnette
{
public:
	// コンストラクタ
    Player(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;

	// 回避行動フラグGet
	bool IsDodging() const { return isDodging; }
    // 衝突関数
    void OnCollisionEnter(GameObject& other) override;
    void OnCollisionStay(GameObject& other) override;
    void OnCollisionExit(GameObject& other) override;
private:
    void Move();
    void Boost();
	void SlowDown();
    void Attack();
    void Dodge();

    // ターゲット
	Target* m_Target = nullptr;
    // カメラ
	MainCamera* m_Camera = nullptr;
    // 弾生成器
	Generator* m_Generator = nullptr; // 弾生成器

    // パラメータ
	Vector3 velocity;// 現在の速度
    float speed = 20.0f; // 移動速度
	float fowardSpeed = 0.0f; // 前方移動速度
	float maxSpeed = 0.0f; // 最大速度
	float minSpeed = 0.0f; // 最小速度
    float rotateSpeed = 2.0f; // 回転速度
    const float boostPower = 20.0f; // ブースト速度
    float dodgeMoveSpeed = 1.5f;      // 回避の移動速度
    bool isDodging = false;
    float dodgeTimer = 0.0f;
    int dodgeDirection = 0; // -1 = 左, 1 = 右
    const float dodgeDuration = 0.3f;
    float dodgeRotateSpeed = 360.0f; // 1秒間に1回転（度）

    float m_SpeedChange = 1.0f;
	float m_DefaultFov = 60.0f; // デフォルトの視野角

    // 上下左右の移動量上限
    Vector2 m_MoveLimit{ 50.0f, 35.0f };
    float smoothLimitRange = 20.0f; // 近づいたら減速が始まる範囲
    float limitFactor = 0.5f; // 移動速度に掛ける減衰率
    //float limitRotate = 50.0f;
	Quaternion baseQuaternion; // 基準のクォータニオン
};