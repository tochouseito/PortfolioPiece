// Enemy.h
#pragma once
#include "Marionnette.h"

#include <random>   // ★追加：乱数

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
    void SetApproachTarget(const math::float3& target, float arriveRadius = 5.0f);

    void OnCollisionEnter(GameObject& other) override;

private:
    void Move();
    void DeadAnimation();
    void BeginDead(); // ★ 死亡開始処理

private:
    EnemySpawner* m_EnemySpawner = nullptr;
    Target* m_Target = nullptr;
    LockOn* m_LockOn = nullptr;

    bool  m_IsActive = false;
    bool  m_IsLockOnTarget = false;
    bool  m_IsDying = false;   // ★ 死亡中フラグ
    float m_Speed = 110.0f;
    math::float3 m_Velocity = math::float3::Zero();

    float m_DeadDuration = 1.5f; // ★ 墜落アニメーションの長さ（秒）
    float m_DeadTimer = 0.0f;    // ★ 残り時間

    // ===== ★追加：3秒ごとランダム旋回 =====
    float m_RandomIntervalSec = 1.0f;   // ★ 3秒ごと
    float m_RandomTimer = 0.0f;

    float m_RandomYawRangeDeg = 35.0f;  // ★ 左右の最大（度）
    float m_RandomPitchRangeDeg = 12.0f;// ★ 上下の最大（度）

    float m_TurnResponse = 6.0f;        // ★ 旋回の追従速度（大きいほどキビキビ）

    math::float3 m_MoveDir = math::float3(0.0f, 0.0f, 1.0f);      // 現在の進行方向（正規化前提）
    math::float3 m_TargetDir = math::float3(0.0f, 0.0f, 1.0f);    // 次の目標方向（正規化前提）

    std::mt19937 m_Rng{}; // ★乱数

    // 生成直後のアプローチ移動
    bool m_IsApproaching = false;
    math::float3 m_ApproachTarget = math::float3::Zero();
    float m_ApproachArriveRadius = 5.0f;
    float m_ApproachSpeed = 140.0f;
    float m_MinAltitude = 20.0f;
};
