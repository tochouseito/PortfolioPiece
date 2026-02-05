#include "Player.h"
using namespace theatriaSystem;
#include "UI/Target.h"
#include "UI/LockOn.h"
#include "Camera/MainCamera.h"
#include "Generator/Generator.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/EnemyMissile.h"
#include "Enemy/Enemy.h"

// Start の処理
void Player::Start()
{
	// 参照取得とタグ設定
	m_Target = GetMarionnette<Target>(L"Target");
	m_Camera = GetMarionnette<MainCamera>(L"MainCamera");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	gameObject.SetTag("Player");

	// パーティクルをプレイヤーに追従させる
	m_Particle = FindGameObjectByName(L"MainParticle");
	if (m_Particle)
	{
		m_Particle->transform->parent = this->gameObject.transform;
	}

	// 初期ステータスとカメラFOVを設定
	m_DefaultScale = transform->scale;
	m_CurrentHp = m_MaxHp;

	Camera camera = m_Camera->GetComponent<Camera>();
	m_DefaultFov = 45.0f;
	camera->fovAngleY = m_DefaultFov;

	// 速度・姿勢の初期化
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

// Update の処理
void Player::Update()
{
	// ダウン中は専用処理
	if (m_IsDown)
	{
		UpdateDownState();
		return;
	}

	// 無敵中は点滅処理を進める
	if (m_IsInvincible)
	{
		UpdateBlink(DeltaTime());
	}

	// 回避中は回避処理のみ行う
	if (isDodging)
	{
		UpdateDodgeState();
		return;
	}

	// 通常状態の更新
	UpdateNormal();
}

// UpdateNormal の処理
void Player::UpdateNormal()
{
	// ===== 通常状態 =====
	// 入力処理（移動・回転）
	Move();

	// 回避開始（Q:左 / E:右）
	if (Input::TriggerKey(DIK_Q))
	{
		isDodging = true;
		dodgeDirection = -1;
		dodgeTimer = dodgeDuration;
		// 直ちに1フレーム目の挙動へ
		Dodge();
		return;
	}
	if (Input::TriggerKey(DIK_E))
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

	// 付随エフェクト更新
	UpdateParticle();
}

// UpdateDodgeState の処理
void Player::UpdateDodgeState()
{
	// 回避挙動の継続と終了判定
	Dodge(); // 継続処理（終了判定込み）
	// ブーストのクールダウンだけは進める
	if (boostCooldownTimer > 0.0f) boostCooldownTimer -= DeltaTime();
	// FOVは回避で弄らない。戻す
	if (m_Camera)
	{
		Camera cam = m_Camera->GetComponent<Camera>();
		cam->fovAngleY = math::Lerp(cam->fovAngleY, m_DefaultFov, 8.0f * DeltaTime());
	}
	UpdateParticle();
}

// UpdateDownState の処理
void Player::UpdateDownState()
{
	const float dt = DeltaTime();

	// 落下を継続
	m_DownTimer += dt;
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	if (rb)
	{
		m_Velocity = math::float3(0.0f, -m_DownFallSpeed, 0.0f);
		rb->velocity = m_Velocity;
	}

	// リスポーン待ち時間を超えたら復帰
	if (m_DownTimer >= m_RespawnDelay)
	{
		Respawn();
	}
}

// ApplyDamage の処理
void Player::ApplyDamage(int damage)
{
	// 無敵中・ダウン中はダメージ無効
	if (m_IsInvincible || m_IsDown)
	{
		return;
	}

	// HP減算とダウン判定
	m_CurrentHp -= damage;
	if (m_CurrentHp <= 0)
	{
		BeginDown();
		return;
	}

	// 被弾後は無敵時間に入る
	m_IsInvincible = true;
	m_InvincibleTimer = m_InvincibleDuration;
	m_BlinkTimer = 0.0f;
	m_BlinkVisible = true;
	transform->scale = m_DefaultScale;
}

// BeginDown の処理
void Player::BeginDown()
{
	// ダウン状態へ遷移
	m_IsDown = true;
	m_IsInvincible = false;
	m_InvincibleTimer = 0.0f;
	m_BlinkTimer = 0.0f;
	m_BlinkVisible = true;
	transform->scale = m_DefaultScale;
	m_DownTimer = 0.0f;
	isDodging = false;
	isBoosting = false;

	// カメラ追従を停止
	if (m_Camera)
	{
		m_Camera->SetFollowEnabled(false);
	}
}

// Respawn の処理
void Player::Respawn()
{
	// 状態とHPを初期化
	m_IsDown = false;
	m_CurrentHp = m_MaxHp;
	m_IsInvincible = true;
	m_InvincibleTimer = m_InvincibleDuration;
	m_BlinkTimer = 0.0f;
	m_BlinkVisible = true;
	transform->scale = m_DefaultScale;

	m_Velocity = math::float3::Zero();
	transform->position.y = minAltitude;

	// 速度のリセット
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	if (rb)
	{
		rb->velocity = m_Velocity;
	}

	// カメラ追従を再開
	if (m_Camera)
	{
		m_Camera->SetFollowEnabled(true);
	}
}

// UpdateBlink の処理
void Player::UpdateBlink(float dt)
{
	// 無敵中のみ点滅を進める
	if (!m_IsInvincible)
	{
		return;
	}

	// 無敵時間と点滅タイマー更新
	m_InvincibleTimer -= dt;
	m_BlinkTimer += dt;
	if (m_BlinkTimer >= m_BlinkInterval)
	{
		m_BlinkTimer = 0.0f;
		m_BlinkVisible = !m_BlinkVisible;
		transform->scale = m_BlinkVisible ? m_DefaultScale : Scale(0.0f, 0.0f, 0.0f);
	}

	if (m_InvincibleTimer <= 0.0f)
	{
		m_IsInvincible = false;
		m_BlinkVisible = true;
		transform->scale = m_DefaultScale;
	}
}

// Move の処理
void Player::Move()
{
	// --- 平行移動 ---
	// 入力をまとめて取得
	const int inputX = (Input::PushKey(DIK_D) ? 1 : 0) + (Input::PushKey(DIK_A) ? -1 : 0);
	const int inputY = (Input::PushKey(DIK_W) ? 1 : 0) + (Input::PushKey(DIK_S) ? -1 : 0);

	// クリア状態では速度を上げる
	if (isClear)
	{
		speed = 400.0f;
	}

	// 前方巡航速度（常にZ+方向へ）
	m_Velocity.z = speed;

	// 入力に追従した速度へ補間（キビキビ）
	float targetVX = static_cast<float>(inputX) * moveMaxX;
	float targetVY = static_cast<float>(inputY) * moveMaxY;
	// クリア演出中は上昇させる
	if (isClear)
	{
		targetVX = 0.0f;
		targetVY = 200.0f;
	}
	const float accelX = (inputX == 0) ? moveDecel : moveAccel;
	const float accelY = (inputY == 0) ? moveDecel : moveAccel;
	const float tX = math::Clamp(accelX * DeltaTime(), 0.0f, 1.0f);
	const float tY = math::Clamp(accelY * DeltaTime(), 0.0f, 1.0f);
	m_Velocity.x = math::Lerp(m_Velocity.x, targetVX, tX);
	m_Velocity.y = math::Lerp(m_Velocity.y, targetVY, tY);

	// 移動制限
	MoveLimit();

	// Rigidbody へ適用
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;

	// ロール（横移動に合わせてなめらかに傾ける）
	const float targetRoll = static_cast<float>(inputX) * rollMaxDeg;
	const float rollSpeed = (inputX == 0) ? rollReturn : rollSmooth;
	transform->degrees.z = math::Lerp(transform->degrees.z, targetRoll, rollSpeed * DeltaTime());

	// ピッチ（縦移動に合わせてなめらかに傾ける）
	const float targetPitch = static_cast<float>(-inputY) * pitchMaxDeg;
	const float pitchSpeed = (inputY == 0) ? pitchReturn : pitchSmooth;
	transform->degrees.x = math::Lerp(transform->degrees.x, targetPitch, pitchSpeed * DeltaTime());

	// --- 回転入力 ---
	// 方向キーで機体の向きを調整
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

// Boost の処理
void Player::Boost()
{
	// クールダウン進行
	if (boostCooldownTimer > 0.0f)
		boostCooldownTimer -= DeltaTime();

	// 回避中は不可（呼ばれない設計だが安全のため）
	if (isDodging) return;

	// 起動
	if (!isBoosting && boostCooldownTimer <= 0.0f && Input::TriggerKey(DIK_SPACE))
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
		if (cam) cam->fovAngleY = math::Lerp(cam->fovAngleY, boostFov, 10.0f * DeltaTime());

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

// SlowDown の処理
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

	// 最低高度の制限
	if (pos.y < minAltitude)
	{
		pos.y = minAltitude;
		transform->position.y = minAltitude;
		if (m_Velocity.y < 0.0f) m_Velocity.y = 0.0f;
	}

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

// Attack の処理
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

// UpdateParticle の処理
void Player::UpdateParticle()
{
	// パーティクルが無い場合は処理しない
	if (!m_Particle)
	{
		return;
	}

	// 位置追従とエミット更新
	m_Particle->transform->position = transform->position;
	Emitter emitter = GetEmitterComponent(*m_Particle);
	emitter->emit = true;
}

// OnCollisionEnter の処理
void Player::OnCollisionEnter(GameObject& other)
{
	// ダウン中は被弾判定をしない
	if (m_IsDown)
	{
		return;
	}

	if (other.GetTag() == "EnemyAttack")
	{
		// 敵弾/ミサイルのダメージを反映
		if (auto bullet = other.GetMarionnette<EnemyBullet>())
		{
			ApplyDamage(bullet->GetDamage());
		}
		else if (auto missile = other.GetMarionnette<EnemyMissile>())
		{
			ApplyDamage(missile->GetDamage());
		}
		else
		{
			ApplyDamage(1);
		}
	}
	else if (other.GetTag() == "Enemy")
	{
		// 敵機接触時のダメージを反映
		if (auto enemy = other.GetMarionnette<Enemy>())
		{
			ApplyDamage(enemy->GetRamDamage());
		}
		else
		{
			ApplyDamage(1);
		}
	}
}

// Dodge の処理
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
	// 距離に応じた減衰係数を計算
	float limitFactor = math::Clamp(distance / smoothLimitRange, 0.0f, 1.0f);
	return m_Acceleration * DeltaTime() * limitFactor;
}

REGISTER_SCRIPT_FACTORY(Player);
