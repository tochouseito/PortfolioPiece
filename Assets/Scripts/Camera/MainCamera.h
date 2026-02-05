#pragma once
#include "Marionnette.h"

class Player;
// メインカメラの追従とイントロ演出を制御するクラス
class MainCamera : public Marionnette
{
public:
	// カメラの位置・回転・FOVをまとめたリグ情報
	struct Rig
	{
		math::float3 pos;
		Quaternion rot;
		float fovDeg;
	};

	// コンストラクタ
    MainCamera(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	void SetFollowEnabled(bool enabled) { m_FollowEnabled = enabled; }
private:
	// ラグ追従
	void LagFollow();
	void UpdateIntroArc(float dt);
	static float EaseInOut(float x) { return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f; }
	static float EaseOut(float x) { return 1.0f - std::pow(1.0f - x, 2.0f); }
	static float Clamp01(float x) { return std::max(0.0f, std::min(1.0f, x)); }

	Player* m_Player = nullptr; // プレイヤーオブジェクトへのポインタ
	bool m_FollowEnabled = true;

	enum class Mode { Intro, BlendToFollow, Follow };
	Mode mode_ = Mode::Intro;

	// イントロ演出パラメータ（お好みで調整）
	float introDuration_ = 2.4f;   // 1カメ→2カメ→3カメの合計時間
	float blendDuration_ = 0.4f;   // イントロ終端→通常追従へのブレンド時間
	float introTime_ = 0.0f;
	float blendTime_ = 0.0f;

	// イントロ用インサートカメラ（ローカル空間）
	static constexpr int kIntroShotCount = 3;
	const math::float3 introShotStartOffsets_[kIntroShotCount] = {
		math::float3(8.0f, 2.0f, 12.0f),   // 1カメ開始
		math::float3(-12.0f, 3.0f, 6.0f),  // 2カメ開始
		math::float3(2.0f, 5.0f, -10.0f),  // 3カメ開始
	};
	const math::float3 introShotEndOffsets_[kIntroShotCount] = {
		math::float3(4.0f, 2.5f, 6.0f),    // 1カメ終了
		math::float3(-8.0f, 3.5f, 2.0f),   // 2カメ終了
		math::float3(-2.0f, 4.0f, -6.0f),  // 3カメ終了
	};

	// ブレンド用キャッシュ
	math::float3 endIntroPos_{};
	Quaternion endIntroRot_{};

	// オフセット
	const math::float3 offset = math::float3(0.0f, 5.0f, -30.0f); // オフセット
	const math::float3 lookOffset = math::float3(0.0f, 2.0f, 10.0f); // 視線の少し前
	const float followSpeed = 5.0f; // 追従速度
	const float rotateSpeed = 5.0f; // 回転速度

	float m_FOVOnBoost = 80.0f; // ブースト時のFOV
	float m_BoostFovTransitionDuration = 1;
	float m_DefaultFOV = 60;
};
