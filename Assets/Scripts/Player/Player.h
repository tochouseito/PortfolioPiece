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
    // 座標取得
    Vector3 GetPosition() { return transform->position; }
    // 速度取得
	float GetSpeed() const { return speed; }
private:
    void Move();
    void Boost();
	void SlowDown();
    // 移動制限
    void MoveLimit();
    void Attack();
    void Dodge();
    // 減衰処理
    float SmoothLimitRange(const float& distance);

    // ターゲット
	Target* m_Target = nullptr;
    // カメラ
	MainCamera* m_Camera = nullptr;
    // 弾生成器
	Generator* m_Generator = nullptr; // 弾生成器

    // ロックオンリスト
	std::unordered_map<std::wstring, GameObject*> m_LockOnMap;

    // 初期値
	Vector3 initialPosition{ 0.0f, 0.0f, 0.0f }; // 初期位置

    // パラメータ
    Vector3 m_Velocity = {};// 現在の速度
	float m_Acceleration = 2.0f; // 加速度
    Vector3 m_AngularVelocity = {};// 角速度
    float m_AngularAcceleration = 2.0f;// 角加速度

    // 減衰
    float damping = 0.9f;// 減衰係数
    Vector2 limitPos{ 30.0f, 50.0f };
    float smoothLimitRange = 10.0f; // 近づいたら減速が始まる範囲
    float curve = 1.0f; // 減衰の強さ


    float speed = 100.0f; // 移動速度
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
    float limitFactor = 0.5f; // 移動速度に掛ける減衰率
    //float limitRotate = 50.0f;
	Quaternion baseQuaternion; // 基準のクォータニオン
};