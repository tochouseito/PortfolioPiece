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

void EnemySpawner::RemoveEnemy(const std::wstring& name)
{
	m_Enemies.remove(name);
}

void EnemySpawner::SpawnEnemy()
{
	// 初期位置
	Vector3 spawnPos = m_Player->GetPosition();
	spawnPos.z += 50.0f;
	GameObject* dst = CloneGameObject(&m_Enemy->gameObject, spawnPos);
	Enemy* enemy = dst->GetMarionnette<Enemy>();
	enemy->SetActive(true);
	m_Enemies.push_back(dst->GetName());
}

REGISTER_SCRIPT_FACTORY(EnemySpawner);