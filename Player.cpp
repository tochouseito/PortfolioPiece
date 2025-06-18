#include "Player.h"
using namespace ChoSystem;
#include "PlayerBulletGenerator.h"

void Player::Start()
{
    // 初期化処理
	m_Target = FindGameObjectByName(L"Target");
	m_Camera = FindGameObjectByName(L"MainCamera");
	m_BulletGenerator = FindGameObjectByName(L"PlayerBulletGenerator");
	m_BulletGeneratorScript = m_BulletGenerator->GetScriptInstance<PlayerBulletGenerator>();

	velocity.Initialize();
}

void Player::Update()
{
    // 毎フレーム処理
	// 入力処理
    Move();
	// ブースト
	Boost();
	// 速度減衰
	SlowDown();
	// 攻撃処理
	Attack();
}

void Player::Move()
{
	// 左右移動
	if (gameObject.input.PushKey(DIK_A))
	{
		velocity.x = -speed * DeltaTime();
	}
	if (gameObject.input.PushKey(DIK_D))
	{
		velocity.x = speed * DeltaTime();
	}
	// 上下移動
	if (gameObject.input.PushKey(DIK_W))
	{
		velocity.y = speed * DeltaTime();
	}
	if (gameObject.input.PushKey(DIK_S))
	{
		velocity.y = -speed * DeltaTime();
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
		velocity.x *= 0.9f;
	}
	if (!gameObject.input.PushKey(DIK_W) && !gameObject.input.PushKey(DIK_S))
	{
		velocity.y *= 0.9f;
	}
}

void Player::Attack()
{
	// 攻撃処理
	if (gameObject.input.TriggerKey(DIK_SPACE))
	{
		// PlayerBulletGeneratorを生成
		m_BulletGeneratorScript = m_BulletGenerator->GetScriptInstance<PlayerBulletGenerator>();
		m_BulletGeneratorScript->GenerateBullet(PLAYER_BULLET_TYPE_NORMAL, 1);
	}
}

REGISTER_SCRIPT_FACTORY(Player);