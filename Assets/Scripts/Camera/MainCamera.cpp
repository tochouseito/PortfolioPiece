#include "MainCamera.h"
using namespace theatriaSystem;
#include "Player/Player.h"

void MainCamera::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	mode_ = Mode::Intro;
	introTime_ = 0.0f;
	blendTime_ = 0.0f;

}

void MainCamera::Update()
{
    // 毎フレーム処理
	const float dt = DeltaTime();
    switch (mode_)
    {
    case Mode::Intro:
        UpdateIntroArc(dt);
        break;

    case Mode::BlendToFollow:
    {
        blendTime_ += dt;
        float t = Clamp01(blendTime_ / blendDuration_);

        // 通常追従の目標姿勢を1フレームだけ計算（反映はブレンド後）
        // 位置は LagFollow と同等の “理想位置” を算出して補間する
        Vector3 followDesiredPos =
            m_Player->transform->position +
            chomath::RotateVector(offset, m_Player->transform->quaternion);

        // 回転は自機向きへ（LagFollowと同等の方針）
        Quaternion followDesiredRot = m_Player->transform->quaternion;

        transform->position = Vector3::Lerp(endIntroPos_, followDesiredPos, t);
        transform->quaternion = Quaternion::Slerp(endIntroRot_, followDesiredRot, t);

        if (t >= 1.0f)
        {
            mode_ = Mode::Follow;
        }
        break;
    }

    case Mode::Follow:
        LagFollow();
        break;
    }
}

void MainCamera::LagFollow()
{
	// プレイヤーの位置からオフセットをかけたターゲット位置を計算
	Vector3 desiredPos = m_Player->transform->position + chomath::RotateVector(offset, m_Player->transform->quaternion);
	// 遅延追従
	transform->position = Vector3::Lerp(transform->position, desiredPos, followSpeed * DeltaTime());
	// カメラの向き
	Vector3 lookTarget = m_Player->transform->position + chomath::RotateVector(lookOffset, m_Player->transform->quaternion);
	Vector3 forward = lookTarget - transform->position;
	// カメラの向きを回転
	Quaternion desiredRot = m_Player->transform->quaternion;
	transform->quaternion = Quaternion::Slerp(transform->quaternion, desiredRot, rotateSpeed * DeltaTime());
}

void MainCamera::UpdateIntroArc(float dt)
{
    introTime_ += dt;
    float t01 = Clamp01(introTime_ / introDuration_);

    // 曲線
    float u = EaseInOut(t01);            // 角度用
    float theta = float(M_PI) * u;           // 0 → π
    float R = std::lerp(Rstart_, Rend_, EaseOut(t01));
    float H = std::lerp(0.0f, Hmax_, u);

    // 自機ローカル座標で円弧：+Z前, +X右, +Y上 を仮定
    Vector3 local;
    local.x = std::sin(theta) * R;          // 右→左へ
    local.z = std::cos(theta) * R;          // 前(θ=0)→背後(θ=π)
    local.y = H;

    // 終盤で背後の所定距離にスムーズに寄せる（0.7→1.0の間でスナップ）
    Vector3 localEnd = { 0.0f, Hmax_, -backEnd_ };
    float snapW = Clamp01((t01 - 0.7f) / 0.3f);
    local = Vector3::Lerp(local, localEnd, snapW);

    // ワールドへ変換
    const auto& ppos = m_Player->transform->position;
    const auto& prot = m_Player->transform->quaternion;
    Vector3 worldPos = ppos + chomath::RotateVector(local, prot);

    // 視線は自機（少し先）を見る
    Vector3 lookTarget = ppos + chomath::RotateVector(Vector3{ 0,0,lookAhead_ }, prot);
    Vector3 fwd = (lookTarget - worldPos).Normalize();
    Vector3 up = Vector3{ 0,1,0 };

	Quaternion lookRot = chomath::MakeLookRotation(fwd, up);

    // 軽いロール（θ速度に応じた演出：開始/終端は弱く中央強め）
    float bankDeg = 10.0f * std::sin(theta) * (1.0f - (1.0f - t01) * (1.0f - t01));
	Quaternion bank = chomath::MakeRotateAxisAngleQuaternion(fwd, chomath::DegreesToRadians(backEnd_));
    Quaternion finalRot = bank * lookRot;

    // 反映
    transform->position = worldPos;
    transform->quaternion = finalRot;

    // 終了判定→通常追従へブレンド
    if (t01 >= 1.0f)
    {
        endIntroPos_ = transform->position;
        endIntroRot_ = transform->quaternion;
        mode_ = Mode::BlendToFollow;
        blendTime_ = 0.0f;
    }
}

REGISTER_SCRIPT_FACTORY(MainCamera);