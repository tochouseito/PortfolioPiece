#pragma once
#include "Marionnette.h"

class Player;
class MainCamera : public Marionnette
{
public:
	struct Rig
	{
		Vector3 pos;
		Quaternion rot;
		float fovDeg;
	};

	// コンストラクタ
    MainCamera(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
	// ラグ追従
	void LagFollow();
	void UpdateIntroArc(float dt);
	static float EaseInOut(float x) { return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f; }
	static float EaseOut(float x) { return 1.0f - std::pow(1.0f - x, 2.0f); }
	static float Clamp01(float x) { return std::max(0.0f, std::min(1.0f, x)); }

	Player* m_Player = nullptr; // プレイヤーオブジェクトへのポインタ

	enum class Mode { Intro, BlendToFollow, Follow };
	Mode mode_ = Mode::Intro;

	// イントロ演出パラメータ（お好みで調整）
	float introDuration_ = 2.4f;   // 0→π の演出時間
	float blendDuration_ = 0.4f;   // イントロ終端→通常追従へのブレンド時間
	float introTime_ = 0.0f;
	float blendTime_ = 0.0f;

	// 軌道形状
	float Rstart_ = 35.0f;   // 開始半径（前方距離）
	float Rend_ = 18.0f;   // 終端近くの半径
	float Hmax_ = 8.0f;    // 最高高度（上方向オフセット）
	float backEnd_ = 16.0f;   // 最終的に背後で取りたい距離
	float lookAhead_ = 6.0f; // 視線の先（自機前方）

	// ブレンド用キャッシュ
	Vector3 endIntroPos_{};
	Quaternion endIntroRot_{};

	// オフセット
	const Vector3 offset = Vector3(0.0f, 5.0f, -30.0f); // オフセット
	const Vector3 lookOffset = Vector3(0.0f, 2.0f, 10.0f); // 視線の少し前
	const float followSpeed = 5.0f; // 追従速度
	const float rotateSpeed = 5.0f; // 回転速度

	float m_FOVOnBoost = 80.0f; // ブースト時のFOV
	float m_BoostFovTransitionDuration = 1;
	float m_DefaultFOV = 60;
};