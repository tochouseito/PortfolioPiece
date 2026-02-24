#include "TitlePlayerAnimation.h"
#include <cmath>
using namespace theatriaSystem;

namespace
{
    constexpr float kTwoPi = 6.283185307179586f;
    constexpr float kHalfPi = 1.5707963267948966f;
    // 使用している周波数倍率(2.1, 0.45, 0.65, 0.9)がすべて一致してループする周期
    constexpr float kCompositeLoopCycles = 20.0f;
    constexpr float kCompositeLoopPhase = kTwoPi * kCompositeLoopCycles;
}

// Start の処理
void TitlePlayerAnimation::Start()
{
    // 初期化処理
	m_BasePos = transform->position; // 今の位置を基準にする
    m_BaseDegrees = transform->degrees; // 今の姿勢を基準にする
	m_Phase = 0.0f;
}

// Update の処理
void TitlePlayerAnimation::Update()
{
    // 毎フレーム処理
    // 位相を進める（角速度 = 2πf）
    m_Phase += kTwoPi * m_FrequencyHz * DeltaTime();
    if (m_Phase > kCompositeLoopPhase)
    {
        // 複合波のループ境界で折り返して、姿勢/位置のスナップを防ぐ
        m_Phase = std::fmod(m_Phase, kCompositeLoopPhase);
    }

    // 単純な上下運動ではなく、複合波でふわっと漂うような動きにする
    const float mainWave = std::sin(m_Phase);
    const float subWave = std::sin(m_Phase * 2.1f + 0.8f);
    const float slowWave = std::sin(m_Phase * 0.45f + 1.1f);

    math::float3 pos = m_BasePos;
    pos.y += m_Amplitude * (mainWave * 0.75f + subWave * 0.25f);
    pos.x += 2.8f * std::sin(m_Phase * 0.65f + kHalfPi * 0.3f);
    pos.z += 1.6f * std::sin(m_Phase * 0.9f + kHalfPi);
    transform->position = pos;

    // 左右スウェイに合わせて機体を軽く傾け、待機感を出す
    math::float3 degrees = m_BaseDegrees;
    degrees.z += 6.0f * std::sin(m_Phase * 0.65f + kHalfPi * 0.8f); // roll
    degrees.x += 3.0f * std::sin(m_Phase + 0.6f) + 1.5f * subWave;   // pitch
    degrees.y += 4.0f * slowWave;                                     // yaw
    transform->degrees = degrees;
}

REGISTER_SCRIPT_FACTORY(TitlePlayerAnimation);
