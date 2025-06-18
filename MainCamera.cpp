#include "MainCamera.h"
using namespace ChoSystem;

void MainCamera::Start()
{
    // 初期化処理
	player = FindGameObjectByName(L"Player");
}

void MainCamera::Update()
{
    // 毎フレーム処理

	// ラグ追従
	LagFollow();
}

void MainCamera::LagFollow()
{
	//Vector3 forward = player->transform.forward();// プレイヤーの前方向
	//forward.Normalize();

	// プレイヤーの位置からオフセットをかけたターゲット位置を計算
	Vector3 desiredPos = player->transform.position() + ChoMath::RotateVector(offset, player->transform.quaternion());
	// 遅延追従
	gameObject.transform.position() = Vector3::Lerp(gameObject.transform.position(), desiredPos, followSpeed * DeltaTime());
	// カメラの向き
	Vector3 lookTarget = player->transform.position() + ChoMath::RotateVector(lookOffset, player->transform.quaternion());
	Vector3 forward = lookTarget - gameObject.transform.position();
	// カメラの向きを回転
	Quaternion desiredRot = ChoMath::MakeLookRotation(forward, Vector3(0.0f, 1.0f, 0.0f));
	gameObject.transform.quaternion() = Quaternion::Slerp(gameObject.transform.quaternion(), desiredRot, rotateSpeed * DeltaTime());
}

REGISTER_SCRIPT_FACTORY(MainCamera);