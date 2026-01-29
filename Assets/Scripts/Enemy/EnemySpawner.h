#pragma once
#include "Marionnette.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include "Vector3.h"
#include <random>

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
	void BeginPhase();
	void AdvancePhase();
	void SpawnPhaseEnemies(int count);
	float3 MakeOffscreenSpawnPosition();
	bool IsOffscreen(const float3& pos) const;

	// テスト用
	void HorizontalSpawn();

    std::vector<float3> LoadEnemySpawnPoints(const std::filesystem::path& jsonPath);

    Player* m_Player = nullptr;
    Enemy* m_Enemy = nullptr;
	class MainCamera* m_MainCamera = nullptr;
	std::unordered_map<std::wstring, Enemy*> m_EnemyMap;
	std::unordered_map<std::wstring, int> m_EnemyPhaseMap;

	std::vector<float3> m_SpawnPoints;

	// フェーズ管理
	static constexpr int kPhaseCount = 5;
	int m_CurrentPhase = 0;
	bool m_PhaseActive = false;
	float m_PhaseTimer = 0.0f;
	float m_ClearWaitTimer = 0.0f;
	float m_PhaseTimeLimit = 20.0f; // 秒
	float m_ClearWaitDuration = 2.0f; // 秒
	std::vector<int> m_PhaseEnemyCounts;
	std::vector<int> m_AlivePerPhase;

	// スポーン調整
	float m_SpawnDepth = 120.0f; // プレイヤー前方
	float m_SpawnHeightOffset = 10.0f;
	float m_SpawnBoundsX = 40.0f; // 左右範囲
	float m_SpawnBoundsY = 120.0f; // 上下範囲
	float m_SpawnOffscreenMargin = 20.0f; // 画面外マージン

	// アプローチ先（プレイヤー前方の範囲）
	float m_ApproachDepth = 140.0f;
	float m_ApproachBoundsX = 30.0f;
	float m_ApproachBoundsY = 100.0f;
	float m_ApproachArriveRadius = 6.0f;

	std::mt19937 m_Rng{};
};
