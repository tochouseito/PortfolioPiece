#include "EnemySpawner.h"
using namespace ChoSystem;
#include "Player/Player.h"
#include "Enemy/Enemy.h"

void EnemySpawner::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Enemy = GetMarionnette<Enemy>(L"Enemy");
}

void EnemySpawner::Update()
{
    // 毎フレーム処理
	if (Input::TriggerKey(DIK_E))
	{
		SpawnEnemy();
	}
}

void EnemySpawner::SpawnEnemy()
{
	GameObject* dst = CloneGameObject(&m_Enemy->gameObject, Vector3(0.0f, 0.0f, 0.0f));
}

REGISTER_SCRIPT_FACTORY(EnemySpawner);