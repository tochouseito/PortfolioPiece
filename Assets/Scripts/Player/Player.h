#pragma once
#include "Marionnette.h"

// 前方宣言
class Target;
class MainCamera;
class Generator;

// プレイヤー機体の操作・状態管理を行うクラス
class Player : public Marionnette
{
public:
	// コンストラクタ
    Player(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	void OnCollisionEnter(GameObject& other) override;

	// 回避行動フラグGet
	bool IsDodging() const { return isDodging; }
    // 座標取得
    math::float3 GetPosition() { return transform->position; }
    // 速度取得
	float GetSpeed() const { return speed; }

    // 
    void SetClear()
    {
		isClear = true;
    }
private:
    void UpdateNormal();
    void UpdateDodgeState();
	void UpdateDownState();
	void ApplyDamage(int damage);
	void BeginDown();
	void Respawn();
	void UpdateBlink(float dt);
    void Move();
    void Boost();
    void SlowDown();
    void MoveLimit();
    void Attack();
    void Dodge();
    void UpdateParticle();
    float SmoothLimitRange(const float& distance);

    // 参照
    Target* m_Target = nullptr;
    MainCamera* m_Camera = nullptr;
    Generator* m_Generator = nullptr;
    GameObject* m_Particle = nullptr;
	Scale m_DefaultScale{};

    std::unordered_map<std::wstring, GameObject*> m_LockOnMap;

    math::float3 initialPosition{ 0.0f, 0.0f, 0.0f };

    // 運動パラメータ
    math::float3 m_Velocity{};              // 現在速度
    float   m_Acceleration = 2.0f;     // 加速度
    math::float3 m_AngularVelocity{};
    float   m_AngularAcceleration = 0.1f;

    // 生存フラグ
    bool m_IsAlive = true;
	bool m_IsDown = false;
	bool m_IsInvincible = false;
	float m_DownTimer = 0.0f;
	float m_InvincibleTimer = 0.0f;
	float m_BlinkTimer = 0.0f;
	bool m_BlinkVisible = true;
	int m_MaxHp = 3;
	int m_CurrentHp = 3;
	float m_RespawnDelay = 2.0f;
	float m_InvincibleDuration = 2.0f;
	float m_BlinkInterval = 0.12f;
	float m_DownFallSpeed = 80.0f;

    // 減衰・制限
    float   damping = 0.9f;
    math::float2 limitPos{ 30.0f, 100.0f };
    float   smoothLimitRange = 10.0f;
    float   curve = 1.0f;

    // 前進速度（ベース）
    float speed = 100.0f; // ベースの巡航前進速度
    float fowardSpeed = 0.0f;   // ※未使用なら後で削除可
    float maxSpeed = 0.0f;
    float minSpeed = 0.0f;

    float rotateSpeed = 20.0f;
    float rollMaxDeg = 20.0f;
    float rollSmooth = 10.0f;
    float rollReturn = 14.0f;
    float pitchMaxDeg = 12.0f;
    float pitchSmooth = 10.0f;
    float pitchReturn = 14.0f;
    float minAltitude = 20.0f;

    float moveMaxX = 80.0f;
    float moveMaxY = 80.0f;
    float moveAccel = 12.0f;
    float moveDecel = 16.0f;

    // === ブースト関連 ===
    const float boostPower = 20.0f;  // 1フレームあたりの追加加速係数（前方へ）
    bool  isBoosting = false;
    float boostTimer = 0.0f;
    const float boostDuration = 0.50f;  // ブースト持続時間（秒）
    float boostCooldownTimer = 0.0f;
    const float boostCooldown = 1.00f;  // 次回ブーストまでの待ち時間（秒）
    float boostFov = 70.0f;  // ブースト時FOV
    float m_DefaultFov = 60.0f;  // 通常FOV

    // === 回避関連 ===
    float dodgeMoveSpeed = 1.5f;   // 回避の移動速度（ローカルX基準）
    bool  isDodging = false;
    float dodgeTimer = 0.0f;
    int   dodgeDirection = 0;      // -1=左 / +1=右
    const float dodgeDuration = 0.30f;
    float dodgeRotateSpeed = 360.0f; // Zロール(度/秒)

    float m_SpeedChange = 1.0f;
    // float m_DefaultFov = 60.0f; // ←上のブースト関連に統合

    float limitFactor = 0.5f;
    Quaternion baseQuaternion;

	bool isClear = false;
};
