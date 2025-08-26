#include "Enemy.h"
using namespace ChoSystem;
#include "Enemy/EnemySpawner.h"

void Enemy::Start()
{
    // 初期化処理
	m_EnemySpawner = GetMarionnette<EnemySpawner>(L"EnemySpawner");
    // タグ設定
	gameObject.SetTag("Enemy");
}

void Enemy::Update()
{
    // 毎フレーム処理
}

// 衝突関数
void Enemy::OnCollisionEnter(GameObject& other)
{
	m_EnemySpawner->RemoveEnemy(gameObject.GetName());
	DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(Enemy);