#pragma once
#include "Marionnette.h"

// 前方宣言
class EnemySpawner;
class Target;
class LockOn;

class Enemy : public Marionnette
{
public:
    Enemy(GameObject& object) : Marionnette(object) {}
    void Start() override;
    void Update() override;

    void SetActive(bool isActive) { m_IsActive = isActive; }
    bool IsActive() const { return m_IsActive; }

    bool IsLockOnTarget() const { return m_IsLockOnTarget; }
    void EnableLockOnTarget(LockOn* lockOn);
    void UnableLockOnTarget();

    void OnCollisionEnter(GameObject& other) override;

private:
    void Move();
    void DeadAnimation();
    void BeginDead(); // ★ 死亡開始処理

    EnemySpawner* m_EnemySpawner = nullptr;
    Target* m_Target = nullptr;
    LockOn* m_LockOn = nullptr;

    bool  m_IsActive = false;
    bool  m_IsLockOnTarget = false;
    bool  m_IsDying = false;   // ★ 死亡中フラグ
    float m_Speed = 110.0f;
    math::float3 m_Velocity = math::float3::Zero();

    float m_DeadDuration = 1.5f;      // ★ 墜落アニメーションの長さ（秒）
    float m_DeadTimer = 0.0f;      // ★ 残り時間
};