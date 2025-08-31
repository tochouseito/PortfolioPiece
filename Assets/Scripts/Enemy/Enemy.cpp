#include "Enemy.h"
using namespace ChoSystem;
#include "Enemy/EnemySpawner.h"
#include "UI/Target.h"
#include "UI/LockOn.h"

void Enemy::Start()
{
    // 初期化処理
	m_EnemySpawner = GetMarionnette<EnemySpawner>(L"EnemySpawner");
	m_Target = GetMarionnette<Target>(L"Target");
    // タグ設定
	gameObject.SetTag("Enemy");
}

void Enemy::Update()
{
    // 毎フレーム処理
}

void Enemy::UnableLockOnTarget()
{ 
	if (!m_IsLockOnTarget) return;
	m_IsLockOnTarget = false;
	m_Target->RemoveLockOn(m_LockOn->gameObject.GetName());
	DestroyGameObject(&m_LockOn->gameObject);
	m_LockOn = nullptr;
}

// 衝突関数
void Enemy::OnCollisionEnter(GameObject& other)
{
	if (other.GetTag() != "PlayerAttack") return;
	m_EnemySpawner->RemoveEnemy(gameObject.GetName());
	UnableLockOnTarget();
	DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(Enemy);