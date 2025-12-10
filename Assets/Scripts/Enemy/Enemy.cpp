#include "Enemy.h"
using namespace theatriaSystem;
#include "Enemy/EnemySpawner.h"
#include "UI/Target.h"
#include "UI/LockOn.h"

void Enemy::Start()
{
	m_EnemySpawner = GetMarionnette<EnemySpawner>(L"EnemySpawner");
	m_Target = GetMarionnette<Target>(L"Target");

	gameObject.SetTag("Enemy");

	// ここで isActive=false なら何もしない
	if (!m_IsActive) { return; }
}

void Enemy::Update()
{
	// 死亡中アニメーションを優先
	if (m_IsDying)
	{
		DeadAnimation();
		return;
	}

	if (!m_IsActive) return;

	Move();
}

void Enemy::EnableLockOnTarget(LockOn* lockOn)
{
	m_IsLockOnTarget = true;
	m_LockOn = lockOn;
}

void Enemy::UnableLockOnTarget()
{
	if (!m_IsLockOnTarget) return;

	m_IsLockOnTarget = false;

	// LockOn UI 側から登録解除
	m_Target->RemoveLockOn(m_LockOn->gameObject.GetName());

	// LockOn UI 自体を破棄
	DestroyGameObject(&m_LockOn->gameObject);
	m_LockOn = nullptr;
}

// 衝突関数
void Enemy::OnCollisionEnter(GameObject& other)
{
	if (other.GetTag() != "PlayerAttack") return;

	// すでに死亡中なら二重処理しない
	if (m_IsDying) return;

	// スポーン管理からはここで抜く（ロジック的にはもう”死んだ”扱い）
	if (m_EnemySpawner)
	{
		m_EnemySpawner->RemoveEnemy(gameObject.GetName());
	}

	// 今ロックオンされていたら解除
	UnableLockOnTarget();

	// ★ 即 Destroy ではなく、死亡アニメ開始
	BeginDead();
}

void Enemy::Move()
{
	// 前方移動
	m_Velocity.z = m_Speed;

	// 適用
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	rb->velocity = m_Velocity;
}

void Enemy::BeginDead()
{
	m_IsDying = true;
	m_IsActive = false;      // 通常の Move は止める
	m_DeadTimer = m_DeadDuration;

	// ここで初期の墜落方向・速度を決める
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	if (rb)
	{
		// 前方＋少し下向きに飛ばす
		math::float3 v = math::float3::Zero();
		v.z = m_Speed * 0.5f;   // 速度は少し遅く
		v.y = -50.0f;           // 下方向に落とす（符号はエンジンの座標系に合わせて調整）
		rb->velocity = v;
	}

	// コライダーを無効化できるならここで無効化する（多重ヒット防止）
	// auto collider = GetComponent<Collider3D>();
	// if (collider) { collider->enabled = false; }
}

void Enemy::DeadAnimation()
{
	// Time::DeltaTime() 的なものがエンジンにある前提
	const float dt = DeltaTime(); // or GetDeltaTime() など、実際のAPIに合わせること

	m_DeadTimer -= dt;

	// 回転させて「墜落感」を出す
	auto transform = GetComponent<Transform>();
	if (transform)
	{
		// ピッチ方向に機首を下げながら、ロールさせる
		transform->degrees.x += 2.0f * dt;  // ラジアン前提なら小さめ、度なら調整
		transform->degrees.z += 4.0f * dt;
	}

	// もし Rigidbody に重力をかけたいならここでも追い加速
	Rigidbody3D rb = GetComponent<Rigidbody3D>();
	if (rb)
	{
		math::float3 v = rb->velocity;
		v.y -= 9.8f * dt * 10.0f; // 適当に強めの重力
		rb->velocity = v;
	}

	// 一定時間経過したらオブジェクトを削除
	if (m_DeadTimer <= 0.0f)
	{
		DestroyGameObject(&gameObject);
	}
}

REGISTER_SCRIPT_FACTORY(Enemy);