#include "EnemySpawner.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Camera/MainCamera.h"
#include "Externals/nlohmann/json.hpp"

void EnemySpawner::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Enemy = GetMarionnette<Enemy>(L"Enemy");
	m_MainCamera = GetMarionnette<MainCamera>(L"MainCamera");

	// EnemySpawnPoint読み込み
	m_SpawnPoints = LoadEnemySpawnPoints("EnemySpawnPoint.json");
	if (m_SpawnPoints.empty())
    {
        m_SpawnPoints = LoadEnemySpawnPoints("Assets/EnemySpawnPoint.json");
    }

	std::random_device rd;
	m_Rng.seed(rd());

	m_PhaseEnemyCounts.clear();
	m_AlivePerPhase.assign(kPhaseCount, 0);
	std::uniform_int_distribution<int> enemyCountDist(5, 10);
	for (int i = 0; i < kPhaseCount; ++i)
	{
		m_PhaseEnemyCounts.push_back(enemyCountDist(m_Rng));
	}
}

void EnemySpawner::Update()
{
    // 毎フレーム処理
	if (!m_Player || !m_Enemy)
	{
		return;
	}

	if (m_CurrentPhase >= kPhaseCount)
	{
		return;
	}

	if (!m_PhaseActive)
	{
		BeginPhase();
	}

	const float dt = DeltaTime();
	m_PhaseTimer += dt;

	if (m_PhaseTimer >= m_PhaseTimeLimit)
	{
		AdvancePhase();
		return;
	}

	if (m_AlivePerPhase[m_CurrentPhase] <= 0)
	{
		m_ClearWaitTimer += dt;
		if (m_ClearWaitTimer >= m_ClearWaitDuration)
		{
			AdvancePhase();
		}
	}
	else
	{
		m_ClearWaitTimer = 0.0f;
	}
}

void EnemySpawner::RemoveEnemy(const std::wstring& name)
{
	m_EnemyMap.erase(name);
	auto it = m_EnemyPhaseMap.find(name);
	if (it != m_EnemyPhaseMap.end())
	{
		const int phaseIndex = it->second;
		if (phaseIndex >= 0 && phaseIndex < static_cast<int>(m_AlivePerPhase.size()))
		{
			if (m_AlivePerPhase[phaseIndex] > 0)
			{
				m_AlivePerPhase[phaseIndex] -= 1;
			}
		}
		m_EnemyPhaseMap.erase(it);
	}
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
		m_EnemyMap[dst->GetName()] = enemy;
	}
}

void EnemySpawner::BeginPhase()
{
	if (m_CurrentPhase >= kPhaseCount)
	{
		return;
	}

	m_PhaseActive = true;
	m_PhaseTimer = 0.0f;
	m_ClearWaitTimer = 0.0f;

	const int count = m_PhaseEnemyCounts[m_CurrentPhase];
	SpawnPhaseEnemies(count);
}

void EnemySpawner::AdvancePhase()
{
	m_PhaseActive = false;
	m_PhaseTimer = 0.0f;
	m_ClearWaitTimer = 0.0f;
	m_CurrentPhase += 1;
}

void EnemySpawner::SpawnPhaseEnemies(int count)
{
	std::uniform_real_distribution<float> xDist(-m_ApproachBoundsX, m_ApproachBoundsX);
	std::uniform_real_distribution<float> yDist(-m_ApproachBoundsY, m_ApproachBoundsY);
	for (int i = 0; i < count; ++i)
	{
		float3 spawnPos = MakeOffscreenSpawnPosition();
		GameObject* dst = CloneGameObject(&m_Enemy->gameObject, spawnPos);
		Enemy* enemy = dst->GetMarionnette<Enemy>();
		enemy->SetActive(true);
		const float3 basePos = m_Player->GetPosition();
		const float3 targetPos(
			basePos.x + xDist(m_Rng),
			basePos.y + yDist(m_Rng),
			basePos.z + m_ApproachDepth
		);
		enemy->SetApproachTarget(targetPos, m_ApproachArriveRadius);
		m_EnemyMap[dst->GetName()] = enemy;
		m_EnemyPhaseMap[dst->GetName()] = m_CurrentPhase;
		m_AlivePerPhase[m_CurrentPhase] += 1;
	}
}

float3 EnemySpawner::MakeOffscreenSpawnPosition()
{
	float3 basePos = m_Player->GetPosition();
	basePos.z += m_SpawnDepth;
	basePos.y += m_SpawnHeightOffset;

	std::uniform_real_distribution<float> xDist(-m_SpawnBoundsX, m_SpawnBoundsX);
	std::uniform_real_distribution<float> yDist(-m_SpawnBoundsY, m_SpawnBoundsY);
	std::uniform_int_distribution<int> edgeDist(0, 3);

	float3 spawnPos = basePos;
	const int edge = edgeDist(m_Rng);
	if (edge == 0)
	{
		spawnPos.x += -m_SpawnBoundsX - m_SpawnOffscreenMargin;
		spawnPos.y += yDist(m_Rng);
	}
	else if (edge == 1)
	{
		spawnPos.x += m_SpawnBoundsX + m_SpawnOffscreenMargin;
		spawnPos.y += yDist(m_Rng);
	}
	else if (edge == 2)
	{
		spawnPos.y += m_SpawnBoundsY + m_SpawnOffscreenMargin;
		spawnPos.x += xDist(m_Rng);
	}
	else
	{
		spawnPos.y += -m_SpawnBoundsY - m_SpawnOffscreenMargin;
		spawnPos.x += xDist(m_Rng);
	}

	if (!IsOffscreen(spawnPos))
	{
		// 画面内に入る場合は少し前方へ押し出す
		spawnPos.z += m_SpawnOffscreenMargin;
	}

	return spawnPos;
}

bool EnemySpawner::IsOffscreen(const float3& pos) const
{
	if (!m_MainCamera)
	{
		return true;
	}

	Camera camera = m_MainCamera->GetComponent<Camera>();
	if (!camera)
	{
		return true;
	}

	const float width = static_cast<float>(ScreenWidth());
	const float height = static_cast<float>(ScreenHeight());
	math::float2 screenPos = math::WorldToScreen(
		math::float3(pos.x, pos.y, pos.z),
		camera->viewMatrix,
		camera->projectionMatrix,
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	);

	return (screenPos.x < 0.0f || screenPos.x > width || screenPos.y < 0.0f || screenPos.y > height);
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
