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

    // ★ 追加：スポーナーから挙動を指定できるようにする
    enum class MovePattern : uint8_t
    {
        Straight = 0,   // 直進
        WeaveYaw,       // S字（ヨー振り）
        SwoopPitch,     // 上下（ピッチ振り）
        ConstantTurn,   // 一定旋回
    };

    struct MoveSettings
    {
        float turnResponse = 6.0f;     // dir追従の速さ（大きいほどキビキビ）
        float weaveYawDeg = 25.0f;     // ヨー最大角（度）
        float weaveFreqHz = 0.6f;      // 周波数（Hz）
        float swoopPitchDeg = 18.0f;   // ピッチ最大角（度）
        float swoopFreqHz = 0.35f;     // 周波数（Hz）
        float constTurnDegPerSec = 40.0f; // 一定旋回（度/秒）
        float bankFactor = 1.2f;       // バンク量（見た目用）
    };

    void SetMovePattern(MovePattern pattern, const MoveSettings& settings) noexcept
    {
        m_MovePattern = pattern;
        m_MoveSettings = settings;
        m_MoveTime = 0.0f;
    }

private:
    void Move();
    void DeadAnimation();
    void BeginDead();

    // ★ 追加：内部ヘルパ
    math::float3 CalcDesiredDir(float dt);
    void ApplyMove(const math::float3& dir);

private:
    EnemySpawner* m_EnemySpawner = nullptr;
    Target* m_Target = nullptr;
    LockOn* m_LockOn = nullptr;

    bool  m_IsActive = false;
    bool  m_IsLockOnTarget = false;
    bool  m_IsDying = false;
    float m_Speed = 110.0f;
    math::float3 m_Velocity = math::float3::Zero();

    float m_DeadDuration = 1.5f;
    float m_DeadTimer = 0.0f;

    // ★ 追加：移動用ステート
    MovePattern  m_MovePattern = MovePattern::Straight;
    MoveSettings m_MoveSettings{};
    float        m_MoveTime = 0.0f;
    math::float3 m_MoveDir = math::float3(0.0f, 0.0f, 1.0f); // 正規化前提
};
