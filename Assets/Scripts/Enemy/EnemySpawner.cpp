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
		// SpawnEnemy();
		HorizontalSpawn();
	}
}

void EnemySpawner::RemoveEnemy(const std::wstring& name)
{
	m_EnemyMap.erase(name);
}

void EnemySpawner::SpawnEnemy()
{
	// 初期位置
	Vector3 spawnPos = m_Player->GetPosition();
	spawnPos.z += 50.0f;
	GameObject* dst = CloneGameObject(&m_Enemy->gameObject, spawnPos);
	Enemy* enemy = dst->GetMarionnette<Enemy>();
	enemy->SetActive(true);
	m_EnemyMap[dst->GetName()] = enemy;
}

void EnemySpawner::HorizontalSpawn()
{
	// プレイヤーの位置を基準に左右にスポーン
	Vector3 basePos = m_Player->GetPosition();
	// プレイヤーより奥にスポーン
	basePos.z += 100.0f;
	basePos.y += 10.0f;
	float spacing = 20.0f; // 敵同士の間隔
	for (int i = -2; i <= 2; ++i)
	{
		Vector3 spawnPos = basePos;
		spawnPos.x += i * spacing;
		GameObject* dst = CloneGameObject(&m_Enemy->gameObject, spawnPos);
		Enemy* enemy = dst->GetMarionnette<Enemy>();
		enemy->SetActive(true);
		m_EnemyMap[dst->GetName()] = enemy;
	}
}

REGISTER_SCRIPT_FACTORY(EnemySpawner);