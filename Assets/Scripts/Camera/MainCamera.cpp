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

	if (Input::TriggerKey(DIK_R))
	{
		mode_ = Mode::Intro;
		introTime_ = 0.0f;
		blendTime_ = 0.0f;
	}

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
        math::float3 followDesiredPos =
            m_Player->transform->position +
            math::RotateVector(offset, m_Player->transform->quaternion);

        // 回転は自機向きへ（LagFollowと同等の方針）
        Quaternion followDesiredRot = m_Player->transform->quaternion;

        transform->position = math::float3::Lerp(endIntroPos_, followDesiredPos, t);
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
    math::float3 desiredPos = m_Player->transform->position + math::RotateVector(offset, m_Player->transform->quaternion);
	// 遅延追従
	transform->position = math::float3::Lerp(transform->position, desiredPos, followSpeed * DeltaTime());
	// カメラの向き
    math::float3 lookTarget = m_Player->transform->position + math::RotateVector(lookOffset, m_Player->transform->quaternion);
    math::float3 forward = lookTarget - transform->position;
	// カメラの向きを回転
	Quaternion desiredRot = m_Player->transform->quaternion;
	transform->quaternion = Quaternion::Slerp(transform->quaternion, desiredRot, rotateSpeed * DeltaTime());
}

void MainCamera::UpdateIntroArc(float dt)
{
	introTime_ += dt;

	const float shotDuration = introDuration_ / static_cast<float>(kIntroShotCount);
	int shotIndex = static_cast<int>(introTime_ / shotDuration);
	if (shotIndex < 0)
	{
		shotIndex = 0;
	}
	else if (shotIndex >= kIntroShotCount)
	{
		shotIndex = kIntroShotCount - 1;
	}

	const float shotTime = introTime_ - (static_cast<float>(shotIndex) * shotDuration);
	const float shotT = Clamp01(shotTime / shotDuration);
	const float easedT = EaseInOut(shotT);

	const auto& ppos = m_Player->transform->position;
	const auto& prot = m_Player->transform->quaternion;

	math::float3 localOffset = math::float3::Lerp(
		introShotStartOffsets_[shotIndex],
		introShotEndOffsets_[shotIndex],
		easedT);
	math::float3 worldPos = ppos + math::RotateVector(localOffset, prot);
	math::float3 lookTarget = ppos;
	math::float3 forward = (lookTarget - worldPos).Normalize();
	math::float3 up = math::float3{ 0,1,0 };

	transform->position = worldPos;
	transform->quaternion = math::MakeLookRotation(forward, up);

	if (introTime_ >= introDuration_)
	{
		endIntroPos_ = transform->position;
		endIntroRot_ = transform->quaternion;
		mode_ = Mode::BlendToFollow;
		blendTime_ = 0.0f;
	}
}

REGISTER_SCRIPT_FACTORY(MainCamera);
