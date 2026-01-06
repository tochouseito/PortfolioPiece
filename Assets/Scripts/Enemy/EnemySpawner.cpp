#include "EnemySpawner.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Externals/nlohmann/json.hpp"

void EnemySpawner::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Enemy = GetMarionnette<Enemy>(L"Enemy");

	// EnemySpawnPoint読み込み
	m_SpawnPoints = LoadEnemySpawnPoints("EnemySpawnPoint.json");
	if (m_SpawnPoints.empty())
    {
        m_SpawnPoints = LoadEnemySpawnPoints("Assets/EnemySpawnPoint.json");
    }
}

void EnemySpawner::Update()
{
    // 毎フレーム処理
	if (Input::TriggerKey(DIK_E))
	{
		//SpawnEnemy();
		HorizontalSpawn();
	}
}

void EnemySpawner::RemoveEnemy(const std::wstring& name)
{
	m_EnemyMap.erase(name);
}

void EnemySpawner::SpawnEnemy()
{
    // プレイヤーの位置を基準に左右にスポーン
    Vector3 basePos = m_Player->GetPosition();
	for (auto& point : m_SpawnPoints)
    {
        Vector3 spawnPos = basePos;
		spawnPos.z += point.z;
        spawnPos.x = point.x * 10.0f;
        GameObject* dst = CloneGameObject(&m_Enemy->gameObject, spawnPos);
        Enemy* enemy = dst->GetMarionnette<Enemy>();
        enemy->SetActive(true);
        m_EnemyMap[dst->GetName()] = enemy;
    }
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
        Enemy::MoveSettings s{};
		s.weaveFreqHz = 0.5f;
		s.weaveYawDeg = 30.0f;
		enemy->SetMovePattern(Enemy::MovePattern::WeaveYaw, s);
		m_EnemyMap[dst->GetName()] = enemy;
	}
}

std::vector<float3> EnemySpawner::LoadEnemySpawnPoints(const std::filesystem::path& jsonPath)
{
    using nlohmann::json;

    // ファイルオープン
    std::ifstream ifs(jsonPath);
    if (!ifs)
    {
        // throw std::runtime_error("Failed to open spawn point file: " + jsonPath.string());
		return {};
    }

    // JSON パース
    json j;
    try
    {
        ifs >> j;
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error(
            "JSON parse error in " + jsonPath.string() + ": " + e.what());
    }

    // fileType チェック（任意だがバグ避けのため）
    if (!j.contains("fileType") || !j["fileType"].is_string() ||
        j["fileType"] != "EnemySpawnPoints")
    {
        throw std::runtime_error(
            "JSON is not EnemySpawnPoints type: " + jsonPath.string());
    }

    // spawnPoints 配列チェック
    if (!j.contains("spawnPoints") || !j["spawnPoints"].is_array())
    {
        throw std::runtime_error(
            "JSON does not contain spawnPoints[]: " + jsonPath.string());
    }

    const auto& spawnArray = j["spawnPoints"];

    std::vector<float3> result;
    result.reserve(spawnArray.size());

    for (const auto& sp : spawnArray)
    {
        // position: [x, y, z]
        if (!sp.contains("position") || !sp["position"].is_array() ||
            sp["position"].size() != 3)
        {
            // 形式おかしいポイントはスキップするなら continue、
            // 厳密にやりたければここで throw に切り替える
            continue;
        }

        const auto& pos = sp["position"];

        float3 v{};
        // JSON 側が double の可能性もあるので float にキャスト
        v.x = static_cast<float>(pos[0].get<double>());
        v.y = static_cast<float>(pos[1].get<double>());
        v.z = static_cast<float>(pos[2].get<double>());

        result.push_back(v);
    }

    return result;
}

REGISTER_SCRIPT_FACTORY(EnemySpawner);