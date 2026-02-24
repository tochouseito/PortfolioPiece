#pragma once
#include <Marionnette.h>

// タイトル画面のプレイヤー演出アニメクラス
class TitlePlayerAnimation : public Marionnette
{
public:
	// コンストラクタ
    TitlePlayerAnimation(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
    float m_Amplitude = 6.0f;        // 上下の基本揺れ幅
    float m_FrequencyHz = 0.65f;     // ややゆっくり目の往復
    float m_Phase = 0.0f;            // 現在の位相[rad]
    math::float3 m_BasePos{};        // 揺れの基準位置
    math::float3 m_BaseDegrees{};    // 揺れの基準姿勢
};
