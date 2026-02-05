#include "Generator.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Player/PlayerBullet.h"
#include "Player/PlayerMissile.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/EnemyMissile.h"
#include "FieldObject/Ground.h"
#include "FieldObject/Wall.h"
#include "Enemy/Enemy.h"

// Start の処理
void Generator::Start()
{
    // 初期化処理
	// 参照取得
	m_Player = GetMarionnette<Player>(L"Player");
	m_PlayerBullet = GetMarionnette<PlayerBullet>(L"PlayerBullet");
	m_PlayerMissile = GetMarionnette<PlayerMissile>(L"PlayerMissile");
	m_EnemyBullet = GetMarionnette<EnemyBullet>(L"EnemyBullet");
	m_EnemyMissile = GetMarionnette<EnemyMissile>(L"EnemyMissile");
	m_Wall = GetMarionnette<Wall>(L"Wall");
	m_Ground = GetMarionnette<Ground>(L"Ground");
}

// Update の処理
void Generator::Update()
{
    // 毎フレーム処理
	// 地形の生成・破棄を管理
    GenerateTerrain();
}

// GeneratePlayerBullet の処理
void Generator::GeneratePlayerBullet()
{
	// プレイヤー位置から弾を複製
	Vector3 pos = m_Player->transform->position;
	GameObject* dst = CloneGameObject(&m_PlayerBullet->gameObject, pos);
	PlayerBullet* bullet = dst->GetMarionnette<PlayerBullet>();
	bullet->SetActive(true);
	// bullet->Start();
	m_PlayerBullets.push_back(dst->GetName());
}

// RemovePlayerBullet の処理
void Generator::RemovePlayerBullet(const std::wstring& name)
{
	m_PlayerBullets.remove(name);
}

// GeneratePlayerMissile の処理
void Generator::GeneratePlayerMissile(Enemy* target)
{
	// ミサイル生成
	// プレイヤーの位置を取得
	Vector3 pos = m_Player->transform->position;
	GameObject* dst = CloneGameObject(&m_PlayerMissile->gameObject, pos);
	PlayerMissile* missile = dst->GetMarionnette<PlayerMissile>();
	missile->SetActive(true);
	missile->SetTargetEnemy(target);
	m_PlayerMissiles.push_back(dst->GetName());
}

// RemovePlayerMissile の処理
void Generator::RemovePlayerMissile(const std::wstring& name)
{
	m_PlayerMissiles.remove(name);
}

// GenerateEnemyBullet の処理
void Generator::GenerateEnemyBullet(const math::float3& position, const math::float3& direction)
{
	// 敵弾の元が無い場合は生成しない
	if (!m_EnemyBullet)
	{
		return;
	}

	// 指定位置・方向で敵弾を生成
	GameObject* dst = CloneGameObject(&m_EnemyBullet->gameObject, position);
	EnemyBullet* bullet = dst->GetMarionnette<EnemyBullet>();
	bullet->SetActive(true);
	bullet->SetDirection(direction);
	m_EnemyBullets.push_back(dst->GetName());
}

// RemoveEnemyBullet の処理
void Generator::RemoveEnemyBullet(const std::wstring& name)
{
	m_EnemyBullets.remove(name);
}

// GenerateEnemyMissile の処理
void Generator::GenerateEnemyMissile(const math::float3& position)
{
	// 敵ミサイルの元が無い場合は生成しない
	if (!m_EnemyMissile)
	{
		return;
	}

	// 指定位置でミサイルを生成
	GameObject* dst = CloneGameObject(&m_EnemyMissile->gameObject, position);
	EnemyMissile* missile = dst->GetMarionnette<EnemyMissile>();
	missile->SetActive(true);
	m_EnemyMissiles.push_back(dst->GetName());
}

// RemoveEnemyMissile の処理
void Generator::RemoveEnemyMissile(const std::wstring& name)
{
	m_EnemyMissiles.remove(name);
}

// GenerateTerrain の処理
void Generator::GenerateTerrain()
{
    // プレイヤーの位置Z
    float playerZ = m_Player->transform->position.z;

    // 現在の地形インデックス
    uint32_t currentIndex = static_cast<uint32_t>(playerZ / m_TerrainOffset.z);

    // プレイヤーの前方に1つ確保する
    uint32_t startIndex = currentIndex;
    uint32_t endIndex = currentIndex + 1;

	// 地形が生成されていない場合、必要な数だけ生成する
    for(uint32_t i = startIndex; i <= endIndex; ++i)
    {
        if(m_TerrainMap.contains(i))
        {
            continue; // 既に生成されている地形はスキップ
		}
		// 新しい地形を生成して登録
        float z = i * m_TerrainOffset.z;
        math::float3 position(m_TerrainOffset.x, m_TerrainOffset.y, z);
        GameObject* newGround = CloneGameObject(&m_Ground->gameObject, position);
        Ground* groundScript = newGround->GetMarionnette<Ground>();
        groundScript->SetScale(Scale(11.0f, 11.0f, 11.0f)); // スケール設定
		m_TerrainMap[i] = newGround;
	}

    // プレイヤーより後方にある地形を削除
    for (auto it = m_TerrainMap.begin();it != m_TerrainMap.end();)
    {
        float terrainZ = it->second->transform->position.z;
		float playerBackZ = playerZ - 500.0f; // プレイヤーの後方500.0fの位置
        if (terrainZ < playerBackZ)
        {
            DestroyGameObject(it->second);
            it = m_TerrainMap.erase(it);
        }
        else
        {
			++it;
        }
    }

}

REGISTER_SCRIPT_FACTORY(Generator);
