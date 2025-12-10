#include "Player.h"
using namespace theatriaSystem;
#include "UI/Target.h"
#include "UI/LockOn.h"
#include "Camera/MainCamera.h"
#include "Generator/Generator.h"

void Player::Start()
{
	m_Target = GetMarionnette<Target>(L"Target");
	m_Camera = GetMarionnette<MainCamera>(L"MainCamera");
	m_Generator = GetMarionnette<Generator>(L"Generator");

	GameObject* particleObj = FindGameObjectByName(L"MainParticle");
	particleObj->transform->parent = this->gameObject.transform;

	Camera camera = m_Camera->GetComponent<Camera>();
	m_DefaultFov = 45.0f;
	camera->fovAngleY = m_DefaultFov;

	m_Velocity.Initialize();
	baseQuaternion.Initialize();

	// REGISTER_FIELD(speed);
	// 必要に応じて UI デバッグ用に以下も公開可
	// REGISTER_FIELD(boostDuration);
	// REGISTER_FIELD(boostCooldown);
	// REGISTER_FIELD(boostFov);
	// REGISTER_FIELD(dodgeDuration);
	// REGISTER_FIELD(dodgeMoveSpeed);
	// REGISTER_FIELD(dodgeRotateSpeed);
}

void Player::Update()
{
	// 回避中は回避処理のみ行う
	if (isDodging)
	{
		Dodge(); // 継続処理（終了判定込み）
		// ブーストのクールダウンだけは進める
		if (boostCooldownTimer > 0.0f) boostCooldownTimer -= DeltaTime();
		// FOVは回避で弄らない。戻す
		if (m_Camera)
		{
			Camera cam = m_Camera->GetComponent<Camera>();
			cam->fovAngleY = math::Lerp(cam->fovAngleY, m_DefaultFov, 8.0f * DeltaTime());
		}
		return;
	}

	// ===== 通常状態 =====
	// 入力処理（移動・回転）
	Move();

	// 回避開始（Q:左 / R:右）
	if (Input::TriggerKey(DIK_Q))
	{
		isDodging = true;
		dodgeDirection = -1;
		dodgeTimer = dodgeDuration;
		// 直ちに1フレーム目の挙動へ
		Dodge();
		return;
	}
	if (Input::TriggerKey(DIK_R))
	{
		isDodging = true;
		dodgeDirection = +1;
		dodgeTimer = dodgeDuration;
		Dodge();
		return;
	}

	// ブースト（開始・継続）
	Boost();

	// 攻撃処理
	Attack();

	GameObject* particleObj = FindGameObjectByName(L"MainParticle");
	particleObj->transform->position = transform->position;
	Emitter emitter = GetEmitterComponent(*particleObj);
	emitter->emit = true;
}

void Player::Move()
{
	// --- 平行移動 ---
	if (Input::PushKey(DIK_A))
	{ 
		m_Velocity.x -= m_Acceleration;
		transform->degrees.z += -1.0f * rotateSpeed;
	}
	if (Input::PushKey(DIK_D))
	{
		m_Velocity.x += m_Acceleration;
		transform->degrees.z += 1.0f * rotateSpeed;
	}
	if (Input::PushKey(DIK_W))
	{
		m_Velocity.y += m_Acceleration;
		transform->degrees.x += -1.0f * rotateSpeed;
	}
	if (Input::PushKey(DIK_S))
	{
		m_Velocity.y -= m_Acceleration;
		transform->degrees.x += 1.0f * rotateSpeed;
	}

	if (isClear)
	{
		speed = 400.0f;
		m_Velocity.y = 200.0f;
	}

	// 前方巡航速度（常にZ+方向へ）
	m_Velocity.z = speed;

	// 移動制限
	MoveLimit();

	// Rigidbody へ適用
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;

	// --- 回転入力 ---
	if (Input::PushKey(DIK_LEFTARROW)) 
	{
		//m_AngularVelocity.y -= m_AngularAcceleration;
		transform->degrees.y += -1.0f * rotateSpeed;
	}
	if (Input::PushKey(DIK_RIGHTARROW))
	{
		//m_AngularVelocity.y += m_AngularAcceleration;
		transform->degrees.y += 1.0f * rotateSpeed;
	}
	if (Input::PushKey(DIK_UPARROW))
	{
		//m_AngularVelocity.x -= m_AngularAcceleration;
		transform->degrees.x += -1.0f * rotateSpeed;
	}
	if (Input::PushKey(DIK_DOWNARROW))
	{
		//m_AngularVelocity.x += m_AngularAcceleration;
		transform->degrees.x += 1.0f * rotateSpeed;
	}

	rb->angularVelocity = m_AngularVelocity;
}

void Player::Boost()
{
	// クールダウン進行
	if (boostCooldownTimer > 0.0f)
		boostCooldownTimer -= DeltaTime();

	// 回避中は不可（呼ばれない設計だが安全のため）
	if (isDodging) return;

	// 起動
	if (!isBoosting && boostCooldownTimer <= 0.0f && Input::TriggerKey(DIK_E))
	{
		isBoosting = true;
		boostTimer = boostDuration;
		boostCooldownTimer = boostCooldown; // 次開始までの待機
	}

	// 継続中の効果
	Camera cam = m_Camera->GetComponent<Camera>();

	if (isBoosting)
	{
		// 前方方向へ強い加速
		const Vector3 forward = math::RotateVector(Vector3(0.0f, 0.0f, 1.0f), transform->quaternion);
		// 「瞬間的インパルス」寄り：DeltaTimeを掛けてフレーム依存性を抑える
		m_Velocity += forward * (boostPower * DeltaTime());

		// FOVをワイド化へ補間
		// if (cam) cam->fovAngleY = chomath::Lerp(cam->fovAngleY, boostFov, 10.0f * DeltaTime());

		// タイマ消化
		boostTimer -= DeltaTime();
		if (boostTimer <= 0.0f)
		{
			isBoosting = false;
		}
	}
	else
	{
		// 非ブースト時はFOVを通常へ戻す
		if (cam) cam->fovAngleY = math::Lerp(cam->fovAngleY, m_DefaultFov, 8.0f * DeltaTime());
	}

	// 速度の最終適用（Moveで適用済みだがブースト加算後にも反映したい場合は再適用）
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;
}

void Player::SlowDown()
{
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
		float t = math::Clamp(distToLimitX / smoothLimitRange, 0.0f, 1.0f);
		m_Velocity.x *= std::pow(t, curve);
	}

	// Y軸 減衰
	if ((m_Velocity.y > 0.0f && pos.y > 0.0f) || (m_Velocity.y < 0.0f && pos.y < 0.0f))
	{
		float distToLimitY = limitPos.y - std::abs(pos.y);
		float t = math::Clamp(distToLimitY / smoothLimitRange, 0.0f, 1.0f);
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
		m_Generator->GeneratePlayerBullet();
	}
	// ミサイル
	if (Input::TriggerKey(DIK_LSHIFT))
	{
		// ロックオンリストを取得
		auto& lockOnMap = m_Target->GetLockOnMap();
		if (lockOnMap.empty()) return; // ロックオンしていなければ終了
		// 全員に発射
		for(auto& pair : lockOnMap)
		{
			LockOn* lockOn = pair.second;
			if (!lockOn) continue;
			if (lockOn->GetIsAttacked()) continue; // 既に攻撃されたならスキップ
			// 弾の生成
			m_Generator->GeneratePlayerMissile(lockOn->GetTargetEnemy());
			lockOn->SetIsAttacked(true); // 攻撃されたフラグを立てる
		}
	}
}

void Player::Dodge()
{
	// すでに isDodging=true で呼ばれる想定。開始は Update() 内の Trigger で行う

	if (isDodging)
	{
		// 残り時間
		dodgeTimer -= DeltaTime();
		if (dodgeTimer <= 0.0f)
		{
			isDodging = false;
			dodgeTimer = 0.0f;

			// 回避終了：速度やロールを軽く減衰・リセット
			m_Velocity *= 0.5f;
			transform->degrees.z = 0.0f;
			return;
		}

		// ロール（Z回転）
		const float rotateDir = (dodgeDirection == 1) ? 1.0f : -1.0f;
		transform->degrees.z += rotateDir * dodgeRotateSpeed * DeltaTime();

		// ローカルX方向へスライド
		const math::float3 dodgeLocal = (dodgeDirection == 1)
			? math::float3(1.0f, 0.0f, 0.0f)  // 右
			: math::float3(-1.0f, 0.0f, 0.0f); // 左

		const math::float3 dodgeWorld = math::RotateVector(dodgeLocal, transform->quaternion);
		m_Velocity = math::float3::Normalize(dodgeWorld) * dodgeMoveSpeed;

		// 前進を残したい場合は以下を加算（任意）
		// m_Velocity += chomath::RotateVector(Vector3(0,0,1), transform->quaternion) * (speed * 0.5f);

		// 物理適用
		Rigidbody3D rb = GetComponent<Rigidbody3D>();
		rb->velocity = m_Velocity;
		return;
	}
}

// 減衰処理
float Player::SmoothLimitRange(const float& distance)
{
	float limitFactor = math::Clamp(distance / smoothLimitRange, 0.0f, 1.0f);
	return m_Acceleration * DeltaTime() * limitFactor;
}

REGISTER_SCRIPT_FACTORY(Player);