#include "Player.h"
using namespace ChoSystem;
#include "UI/Target.h"
#include "Camera/MainCamera.h"
#include "Generator/Generator.h"

void Player::Start()
{
	// 初期化処理
	// ターゲットを取得
	m_Target = GetMarionnette<Target>(L"Target");
	m_Camera = GetMarionnette<MainCamera>(L"MainCamera");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	Camera camera = m_Camera->GetComponent<Camera>();
	camera->fovAngleY = 45.0f; // FOVを設定
	m_Velocity.Initialize();
	baseQuaternion.Initialize();
}

void Player::Update()
{
	// 毎フレーム処理
	if (!isDodging)
	{
		// 入力処理
		Move();
		// ブースト
		//Boost();
		// 速度減衰
		//SlowDown();
		// 攻撃処理
		Attack();
	}
	// 回避処理
	//Dodge();
	// 位置更新
	//gameObject.transform.position() += velocity;
	// FOV更新
	//m_Camera->camera.fovAngleY() = m_DefaultFov + speed * 0.5f;
}

void Player::OnCollisionEnter(GameObject& other)
{
	// test
	other;
	int i = 0;
	i++;
}

void Player::OnCollisionStay(GameObject& other)
{
	// test
	other;
	int i = 0;
	i++;
}

void Player::OnCollisionExit(GameObject& other)
{
	// test
	other;
	int i = 0;
	i++;
}

void Player::Move()
{
	// 移動処理

	// 左右移動
	if(Input::PushKey(DIK_A))
	{
		m_Velocity.x -= m_Acceleration;
	}
	if(Input::PushKey(DIK_D))
	{
		m_Velocity.x += m_Acceleration;
	}
	// 上下移動
	if(Input::PushKey(DIK_W))
	{
		m_Velocity.y += m_Acceleration;
	}
	if(Input::PushKey(DIK_S))
	{
		m_Velocity.y -= m_Acceleration;
	}

	// 移動制限
	// MoveLimit();

	// 前方移動
	m_Velocity.z = speed;

	// 適用
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;

	// 回転

	if (Input::PushKey(DIK_LEFTARROW))
	{
		m_AngularVelocity.y += m_AngularAcceleration;
	}

	if (Input::PushKey(DIK_RIGHTARROW))
	{
		m_AngularVelocity.y -= m_AngularAcceleration;
	}

	if (Input::PushKey(DIK_UPARROW))
	{
		m_AngularVelocity.x += m_AngularAcceleration;
	}

	if (Input::PushKey(DIK_DOWNARROW))
	{
		m_AngularVelocity.x -= m_AngularAcceleration;
	}

	// 適用
	rb->angularVelocity = m_AngularVelocity;

	// 回転
	// Z軸回転
	//if (Input::PushKey(DIK_A))
	//{
	//	transform->degrees.z += rotateSpeed * DeltaTime();
	//}
	//if (Input::PushKey(DIK_D))
	//{
	//	transform->degrees.z -= rotateSpeed * DeltaTime();
	//}
	////// 左右回転
	////if (gameObject.input.PushKey(DIK_LEFT))
	////{
	////	gameObject.transform.rotation().y -= rotateSpeed * DeltaTime();
	////}
	////if (gameObject.input.PushKey(DIK_RIGHT))
	////{
	////	gameObject.transform.rotation().y += rotateSpeed * DeltaTime();
	////}
	////// 上下回転
	////if (gameObject.input.PushKey(DIK_UP))
	////{
	////	gameObject.transform.rotation().x -= rotateSpeed * DeltaTime();
	////}
	////if (gameObject.input.PushKey(DIK_DOWN))
	////{
	////	gameObject.transform.rotation().x += rotateSpeed * DeltaTime();
	////}
	//// 左右移動
	//Vector3 pos = transform->position;
	//// 移動減衰処理
	//std::function<float(const float&)> smoothLimitRangeFunc = [this](const float& distance) {
	//	float limitFactor = chomath::Clamp(distance / smoothLimitRange, 0.0f, 1.0f);
	//	return speed * DeltaTime() * limitFactor;
	//	};
	//// 目標傾き角度
	//float roll = 0.0f;// Z軸(ロール)の傾き角度
	//float yaw = 0.0f; // Y軸(ヨー)の傾き角度
	//float pitch = 0.0f;// X軸(ピッチ)の傾き角度
	//if (Input::PushKey(DIK_UP))
	//{
	//	pos.y += smoothLimitRangeFunc(m_MoveLimit.y - pos.y);
	//	pitch = -30.0f; // 上に傾く
	//}
	//else if (Input::PushKey(DIK_DOWN))
	//{
	//	pos.y -= smoothLimitRangeFunc(pos.y + m_MoveLimit.y);
	//	pitch = 30.0f; // 下に傾く
	//}
	//if (Input::PushKey(DIK_LEFT))
	//{
	//	pos.x -= smoothLimitRangeFunc(pos.x + m_MoveLimit.x);
	//	roll = 50.0f; // 左に傾く
	//	pitch = -10.0f; // 左斜め前に向ける
	//}
	//else if (Input::PushKey(DIK_RIGHT))
	//{
	//	pos.x += smoothLimitRangeFunc(m_MoveLimit.x - pos.x);
	//	roll = -50.0f; // 右に傾く
	//	pitch = -10.0f;   // 右斜め前に向ける（同じく下げる）
	//}
	//if (Input::PushKey(DIK_UP) &&
	//	Input::PushKey(DIK_LEFT) &&
	//	Input::PushKey(DIK_DOWN) &&
	//	Input::PushKey(DIK_RIGHT))
	//{
	//	pitch = 0.0f; // 上下左右同時押しで傾きをリセット
	//	roll = 0.0f; // ロールもリセット
	//	yaw = 0.0f; // ヨーもリセット
	//}
	//if(!roll && !pitch)
	//{
	//	transform->quaternion = Quaternion::Slerp(transform->quaternion, baseQuaternion, rotateSpeed * DeltaTime());
	//}
	//// 上下左右の範囲制限を適用
	//pos.x = chomath::Clamp(pos.x, -m_MoveLimit.x, m_MoveLimit.x);
	//pos.y = chomath::Clamp(pos.y, -m_MoveLimit.y, m_MoveLimit.y);
	//// 傾きを徐々に追従
	//transform->degrees.z = chomath::Lerp(transform->degrees.z, roll, rotateSpeed * DeltaTime());
	//transform->degrees.y = chomath::Lerp(transform->degrees.y, yaw, rotateSpeed * DeltaTime());
	//transform->degrees.x = chomath::Lerp(transform->degrees.x, pitch, rotateSpeed * DeltaTime());
	//// 適用
	//transform->position = pos;
	//
	//// 速度上昇
	//if (Input::PushKey(DIK_W))
	//{
	//	// 前進
	//	if (speed < maxSpeed)
	//	{
	//		speed++;
	//	}
	//	else
	//	{
	//		speed = maxSpeed;
	//	}
	//}
	//// 速度減少
	//if (Input::PushKey(DIK_S))
	//{
	//	// 後退
	//	if (speed > minSpeed)
	//	{
	//		speed--;
	//	}
	//	else
	//	{
	//		speed = minSpeed;
	//	}
	//}

	//Vector3 forward = chomath::RotateVector(Vector3(0.0f, 0.0f, 1.0f), transform->quaternion);
	//velocity += Vector3::Normalize(forward) * (fowardSpeed * DeltaTime());
	////Rigidbody3D rb = GetComponent<Rigidbody3D>();
	////rb->velocity = velocity;
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


// 移動制限
void Player::MoveLimit()
{
	// X軸（左右）の減衰
	if (!Input::PushKey(DIK_A) && !Input::PushKey(DIK_D))
	{
		m_Velocity.x *= damping;
		// 一定以下になったら停止
		if (std::abs(m_Velocity.x) < 0.01f) m_Velocity.x = 0.0f;
	}

	// Y軸（上下）の減衰
	if (!Input::PushKey(DIK_W) && !Input::PushKey(DIK_S))
	{
		m_Velocity.y *= damping;
		if (std::abs(m_Velocity.y) < 0.01f) m_Velocity.y = 0.0f;
	}

	// 移動制限
	Vector3 pos = transform->position;

	// 壁に近づいている方向にだけ減衰
	// X軸 減衰
	if ((m_Velocity.x > 0.0f && pos.x > 0.0f) || (m_Velocity.x < 0.0f && pos.x < 0.0f))
	{
		float distToLimitX = limitPos.x - std::abs(pos.x);
		float t = chomath::Clamp(distToLimitX / smoothLimitRange, 0.0f, 1.0f);
		m_Velocity.x *= std::pow(t, curve);
	}

	// Y軸 減衰
	if ((m_Velocity.y > 0.0f && pos.y > 0.0f) || (m_Velocity.y < 0.0f && pos.y < 0.0f))
	{
		float distToLimitY = limitPos.y - std::abs(pos.y);
		float t = chomath::Clamp(distToLimitY / smoothLimitRange, 0.0f, 1.0f);
		m_Velocity.y *= std::pow(t, curve);
	}

	// 範囲外に出るなら速度を0に
	if (pos.x <= -limitPos.x && m_Velocity.x < 0.0f) m_Velocity.x = 0.0f;
	if (pos.x >= limitPos.x && m_Velocity.x > 0.0f) m_Velocity.x = 0.0f;
	if (pos.y <= -limitPos.y && m_Velocity.y < 0.0f) m_Velocity.y = 0.0f;
	if (pos.y >= limitPos.y && m_Velocity.y > 0.0f) m_Velocity.y = 0.0f;
}

void Player::Attack()
{
	// 攻撃処理
	// マシンガン
	if (Input::TriggerKey(DIK_SPACE))
	{
		// 弾の生成
		m_Generator->GeneratePlayerBullet(PLAYER_BULLET_TYPE_NORMAL, transform->position);
	}
	// ミサイル
	if (Input::TriggerKey(DIK_LSHIFT))
	{
		// 弾の生成
		m_Generator->GeneratePlayerBullet(PLAYER_BULLET_TYPE_MISSILE, transform->position);
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
			m_Velocity *= 0.5f;
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
		m_Velocity = Vector3::Normalize(dir) * dodgeMoveSpeed;
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

// 減衰処理
float Player::SmoothLimitRange(const float& distance)
{
	float limitFactor = chomath::Clamp(distance / smoothLimitRange, 0.0f, 1.0f);
	return m_Acceleration * DeltaTime() * limitFactor;
}

REGISTER_SCRIPT_FACTORY(Player);