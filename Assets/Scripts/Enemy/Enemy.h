#pragma once
#include "Marionnette.h"

// 前方宣言
class EnemySpawner;
class Target;
class LockOn;

class Enemy : public Marionnette
{
public:
	// コンストラクタ
    Enemy(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// アクティブ設定
	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }
	// ロックオン設定
	bool IsLockOnTarget() const { return m_IsLockOnTarget; }
	void EnableLockOnTarget(LockOn* lockOn);
	void UnableLockOnTarget();
	// 衝突関数
	void OnCollisionEnter(GameObject& other) override;
private:
	void Move();

	EnemySpawner* m_EnemySpawner = nullptr;
	Target* m_Target = nullptr;
	LockOn* m_LockOn = nullptr;

    bool m_IsActive = false;
	bool m_IsLockOnTarget = false;// ロックオンされているか
	float m_Speed = 110.0f;// 移動速度
	math::float3 m_Velocity = math::float3::Zero();// 速度
};