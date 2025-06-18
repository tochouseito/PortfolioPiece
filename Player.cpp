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
	if (!isDodging)
	{
		// 入力処理
		Move();
		// ブースト
		Boost();
		// 速度減衰
		SlowDown();
		// 攻撃処理
		Attack();
	}
	// 回避処理
	Dodge();
	// 位置更新
	gameObject.transform.position() += velocity;
}

void Player::Move()
{
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
	Vector3 forward = ChoMath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), gameObject.transform.quaternion());
	gameObject.transform.position() += Vector3::Normalize(forward) * (speed * DeltaTime());
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

void Player::Dodge()
{
	if (isDodging)
	{
		// 回避時間カウント
		dodgeTimer -= DeltaTime();
		if (dodgeTimer <= 0.0f)
		{
			isDodging = false;
			dodgeTimer = 0.0f;

			// 回避終了：速度やロールをリセット
			velocity *= 0.5f;
			gameObject.transform.rotation().z = 0.0f; // 傾きを元に戻す（任意）
			return;
		}

		// 回避方向に連続回転 + 移動
		float rotateDir = (dodgeDirection == 1) ? 1.0f : -1.0f;
		gameObject.transform.rotation().z += rotateDir * dodgeRotateSpeed * DeltaTime();

		Vector3 dodgeVec = (dodgeDirection == 1)
			? Vector3(1.0f, 0.0f, 0.0f)  // 右
			: Vector3(-1.0f, 0.0f, 0.0f); // 左

		Vector3 dir = ChoMath::RotateVector(dodgeVec, gameObject.transform.quaternion());
		velocity = Vector3::Normalize(dir) * dodgeMoveSpeed;
		return;
	}

	// 回避入力検出（初回トリガー）
	if (gameObject.input.TriggerKey(DIK_E))
	{
		isDodging = true;
		dodgeTimer = dodgeDuration;
		dodgeDirection = 1; // 右回避
	}
	else if (gameObject.input.TriggerKey(DIK_Q))
	{
		isDodging = true;
		dodgeTimer = dodgeDuration;
		dodgeDirection = -1; // 左回避
	}
}

REGISTER_SCRIPT_FACTORY(Player);