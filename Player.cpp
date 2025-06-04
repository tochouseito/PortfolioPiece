#include "Player.h"
using namespace ChoSystem;

void Player::Start()
{
    // 初期化処理
}

void Player::Update()
{
    // 毎フレーム処理
    Move();
}

void Player::Move()
{
	// 左右移動
	if (gameObject.input.PushKey(DIK_A))
	{
		gameObject.rigidbody2D.velocity().x = -speed * DeltaTime();
	}
	if (gameObject.input.PushKey(DIK_D))
	{
		gameObject.rigidbody2D.velocity().x = speed * DeltaTime();
	}
	// 上下移動
	if (gameObject.input.PushKey(DIK_W))
	{
		gameObject.rigidbody2D.velocity().y = speed * DeltaTime();
	}
	if (gameObject.input.PushKey(DIK_S))
	{
		gameObject.rigidbody2D.velocity().y = -speed * DeltaTime();
	}
	// 回転
	// 左右回転
	if (gameObject.input.PushKey(DIK_LEFT))
	{
		gameObject.transform.rotation().y -= rotateSpeed * DeltaTime();
	}
	if (gameObject.input.PushKey(DIK_RIGHT))
	{
		gameObject.transform.rotation().y += rotateSpeed * DeltaTime();
	}
	// 上下回転
	if (gameObject.input.PushKey(DIK_UP))
	{
		gameObject.transform.rotation().x -= rotateSpeed * DeltaTime();
	}
	if (gameObject.input.PushKey(DIK_DOWN))
	{
		gameObject.transform.rotation().x += rotateSpeed * DeltaTime();
	}
	// ブースト
	Boost();
	// 速度減衰
	SlowDown();
}

void Player::Boost()
{
	if (gameObject.input.PushKey(DIK_SPACE))
	{
		// ブースト中
		// forwardを取得
		Vector3 forward = ChoMath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), gameObject.transform.quaternion());
		// forwardにブーストをかける
		Vector3 boost = forward * boostPower * DeltaTime();
	}
}

void Player::SlowDown()
{
	// 非入力時速度減衰
	if (!gameObject.input.PushKey(DIK_A) && !gameObject.input.PushKey(DIK_D))
	{
		gameObject.rigidbody2D.velocity().x *= 0.9f;
	}
	if (!gameObject.input.PushKey(DIK_W) && !gameObject.input.PushKey(DIK_S))
	{
		gameObject.rigidbody2D.velocity().y *= 0.9f;
	}
}

REGISTER_SCRIPT_FACTORY(Player);