#include "Player.h"
using namespace ChoSystem;
#include "PlayerBulletGenerator.h"
#include "Camera/MainCamera.h"
#include <functional>

void Player::Start()
{
	// 初期化処理
	/*m_Target = &FindGameObjectByName(L"Target");
	m_Camera = &FindGameObjectByName(L"MainCamera");
	m_BulletGenerator = &FindGameObjectByName(L"PlayerBulletGenerator");
	m_BulletGeneratorScript = m_BulletGenerator->GetScriptInstance<PlayerBulletGenerator>();*/
	//m_Camera = FindGameObjectByName(L"TPSCamera");
	m_Camera = GetMarionnette<MainCamera>(L"TPSCamera");
	Camera camera = m_Camera->GetComponent<Camera>();
	camera->fovAngleY = 45.0f; // FOVを設定
	//m_Camera->camera.fovAngleY() = 45.0f;
	velocity.Initialize();
	baseQuaternion.Initialize();
}

void Player::Update()
{
	m_Target = FindGameObjectByName(L"Target");
	//m_Camera = FindGameObjectByName(L"TPSCamera");
	m_BulletGenerator = FindGameObjectByName(L"PlayerBulletGenerator");
	m_BulletGeneratorScript = m_BulletGenerator->GetScriptInstance<PlayerBulletGenerator>();
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
	//gameObject.transform.position() += velocity;
	// FOV更新
	//m_Camera->camera.fovAngleY() = m_DefaultFov + speed * 0.5f;
}

void Player::Move()
{
	// 回転
	// Z軸回転
	if (Input::PushKey(DIK_A))
	{
		transform->degrees.z += rotateSpeed * DeltaTime();
	}
	if (Input::PushKey(DIK_D))
	{
		transform->degrees.z -= rotateSpeed * DeltaTime();
	}
	//// 左右回転
	//if (gameObject.input.PushKey(DIK_LEFT))
	//{
	//	gameObject.transform.rotation().y -= rotateSpeed * DeltaTime();
	//}
	//if (gameObject.input.PushKey(DIK_RIGHT))
	//{
	//	gameObject.transform.rotation().y += rotateSpeed * DeltaTime();
	//}
	//// 上下回転
	//if (gameObject.input.PushKey(DIK_UP))
	//{
	//	gameObject.transform.rotation().x -= rotateSpeed * DeltaTime();
	//}
	//if (gameObject.input.PushKey(DIK_DOWN))
	//{
	//	gameObject.transform.rotation().x += rotateSpeed * DeltaTime();
	//}
	// 左右移動
	Vector3 pos = transform->position;
	// 移動減衰処理
	std::function<float(const float&)> smoothLimitRangeFunc = [this](const float& distance) {
		float limitFactor = chomath::Clamp(distance / smoothLimitRange, 0.0f, 1.0f);
		return speed * DeltaTime() * limitFactor;
		};
	// 目標傾き角度
	float roll = 0.0f;// Z軸(ロール)の傾き角度
	float yaw = 0.0f; // Y軸(ヨー)の傾き角度
	float pitch = 0.0f;// X軸(ピッチ)の傾き角度
	if (Input::PushKey(DIK_UP))
	{
		pos.y += smoothLimitRangeFunc(m_MoveLimit.y - pos.y);
		pitch = -30.0f; // 上に傾く
	}
	else if (Input::PushKey(DIK_DOWN))
	{
		pos.y -= smoothLimitRangeFunc(pos.y + m_MoveLimit.y);
		pitch = 30.0f; // 下に傾く
	}
	if (Input::PushKey(DIK_LEFT))
	{
		pos.x -= smoothLimitRangeFunc(pos.x + m_MoveLimit.x);
		roll = 50.0f; // 左に傾く
		pitch = -10.0f; // 左斜め前に向ける
	}
	else if (Input::PushKey(DIK_RIGHT))
	{
		pos.x += smoothLimitRangeFunc(m_MoveLimit.x - pos.x);
		roll = -50.0f; // 右に傾く
		pitch = -10.0f;   // 右斜め前に向ける（同じく下げる）
	}
	if (Input::PushKey(DIK_UP) &&
		Input::PushKey(DIK_LEFT) &&
		Input::PushKey(DIK_DOWN) &&
		Input::PushKey(DIK_RIGHT))
	{
		pitch = 0.0f; // 上下左右同時押しで傾きをリセット
		roll = 0.0f; // ロールもリセット
		yaw = 0.0f; // ヨーもリセット
	}
	if(!roll && !pitch)
	{
		transform->quaternion = Quaternion::Slerp(transform->quaternion, baseQuaternion, rotateSpeed * DeltaTime());
	}
	// 上下左右の範囲制限を適用
	pos.x = chomath::Clamp(pos.x, -m_MoveLimit.x, m_MoveLimit.x);
	pos.y = chomath::Clamp(pos.y, -m_MoveLimit.y, m_MoveLimit.y);
	// 傾きを徐々に追従
	transform->degrees.z = chomath::Lerp(transform->degrees.z, roll, rotateSpeed * DeltaTime());
	transform->degrees.y = chomath::Lerp(transform->degrees.y, yaw, rotateSpeed * DeltaTime());
	transform->degrees.x = chomath::Lerp(transform->degrees.x, pitch, rotateSpeed * DeltaTime());
	// 適用
	transform->position = pos;
	
	// 速度上昇
	if (Input::PushKey(DIK_W))
	{
		// 前進
		if (speed < maxSpeed)
		{
			speed++;
		}
		else
		{
			speed = maxSpeed;
		}
	}
	// 速度減少
	if (Input::PushKey(DIK_S))
	{
		// 後退
		if (speed > minSpeed)
		{
			speed--;
		}
		else
		{
			speed = minSpeed;
		}
	}

	Vector3 forward = chomath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), transform->quaternion);
	transform->position += Vector3::Normalize(forward) * (fowardSpeed * DeltaTime());
}

void Player::Boost()
{
	if (Input::PushKey(DIK_SPACE))
	{
		// ブースト中
		// forwardを取得
		Vector3 forward = chomath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), transform->quaternion);
		// forwardにブーストをかける
		Vector3 boost = forward * boostPower * DeltaTime();
	}
}

void Player::SlowDown()
{
	//// 非入力時速度減衰
	//if (!gameObject.input.PushKey(DIK_A) && !gameObject.input.PushKey(DIK_D))
	//{
	//	velocity.x *= 0.9f;
	//}
	//if (!gameObject.input.PushKey(DIK_W) && !gameObject.input.PushKey(DIK_S))
	//{
	//	velocity.y *= 0.9f;
	//}
}

void Player::Attack()
{
	// 攻撃処理
	if (Input::TriggerKey(DIK_SPACE))
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
			transform->degrees.z = 0.0f; // 傾きを元に戻す（任意）
			return;
		}

		// 回避方向に連続回転 + 移動
		float rotateDir = (dodgeDirection == 1) ? 1.0f : -1.0f;
		transform->degrees.z += rotateDir * dodgeRotateSpeed * DeltaTime();

		Vector3 dodgeVec = (dodgeDirection == 1)
			? Vector3(1.0f, 0.0f, 0.0f)  // 右
			: Vector3(-1.0f, 0.0f, 0.0f); // 左

		Vector3 dir = chomath::RotateVector(dodgeVec, transform->quaternion);
		velocity = Vector3::Normalize(dir) * dodgeMoveSpeed;
		return;
	}

	// 回避入力検出（初回トリガー）
	//if (gameObject.input.TriggerKey(DIK_E))
	//{
	//	isDodging = true;
	//	dodgeTimer = dodgeDuration;
	//	dodgeDirection = 1; // 右回避
	//}
	//else if (gameObject.input.TriggerKey(DIK_Q))
	//{
	//	isDodging = true;
	//	dodgeTimer = dodgeDuration;
	//	dodgeDirection = -1; // 左回避
	//}
}

REGISTER_SCRIPT_FACTORY(Player);