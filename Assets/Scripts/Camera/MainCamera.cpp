#include "MainCamera.h"
using namespace ChoSystem;
#include "../Player.h"

void MainCamera::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
}

void MainCamera::Update()
{
    // 毎フレーム処理
	// ポインタを取得

	// ラグ追従
	LagFollow();
}

void MainCamera::LagFollow()
{
	// プレイヤーの位置からオフセットをかけたターゲット位置を計算
	Vector3 desiredPos = m_Player->transform->position + ChoMath::RotateVector(offset, m_Player->transform->quaternion);
	// 遅延追従
	transform->position = Vector3::Lerp(transform->position, desiredPos, followSpeed * DeltaTime());
	// カメラの向き
	Vector3 lookTarget = m_Player->transform->position + ChoMath::RotateVector(lookOffset, m_Player->transform->quaternion);
	Vector3 forward = lookTarget - transform->position;
	// カメラの向きを回転
	Quaternion desiredRot = m_Player->transform->quaternion;
	transform->quaternion = Quaternion::Slerp(transform->quaternion, desiredRot, rotateSpeed * DeltaTime());
}

REGISTER_SCRIPT_FACTORY(MainCamera);