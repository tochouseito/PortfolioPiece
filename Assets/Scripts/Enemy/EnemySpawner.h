#pragma once
#include "Marionnette.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include "Vector3.h"

// 前方宣言
class Enemy;
class Player;

class EnemySpawner : public Marionnette
{
public:
	// コンストラクタ
    EnemySpawner(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
    
    void RemoveEnemy(const std::wstring& name);
	std::unordered_map<std::wstring, Enemy*>& GetEnemyMap() { return m_EnemyMap; }
private:
    void SpawnEnemy();

    // テスト用
	void HorizontalSpawn();

    std::vector<float3> LoadEnemySpawnPoints(const std::filesystem::path& jsonPath);

    Player* m_Player = nullptr;
    Enemy* m_Enemy = nullptr;
	std::unordered_map<std::wstring, Enemy*> m_EnemyMap;

	std::vector<float3> m_SpawnPoints;
};