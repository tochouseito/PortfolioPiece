#pragma once
#include "Marionnette.h"
#include <list>
#include <string>

// 前方宣言
class Player;
class PlayerBullet;
class PlayerMissile;
class EnemyBullet;
class EnemyMissile;
class Wall;
class Ground;
class Enemy;

class Generator : public Marionnette
{
public:
	// コンストラクタ
    Generator(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// プレイヤー弾生成
	void GeneratePlayerBullet();
	// プレイヤー弾削除
	void RemovePlayerBullet(const std::wstring& name);
	// プレイヤーミサイル生成
	void GeneratePlayerMissile(Enemy* target);
	// プレイヤーミサイル削除
	void RemovePlayerMissile(const std::wstring& name);
	// 敵弾生成
	void GenerateEnemyBullet(const math::float3& position, const math::float3& direction);
	// 敵弾削除
	void RemoveEnemyBullet(const std::wstring& name);
	// 敵ミサイル生成
	void GenerateEnemyMissile(const math::float3& position);
	// 敵ミサイル削除
	void RemoveEnemyMissile(const std::wstring& name);
private:
	// 地形生成
	void GenerateTerrain();

	Player* m_Player = nullptr;
	PlayerBullet* m_PlayerBullet = nullptr;
	PlayerMissile* m_PlayerMissile = nullptr;
	EnemyBullet* m_EnemyBullet = nullptr;
	EnemyMissile* m_EnemyMissile = nullptr;
	Wall* m_Wall = nullptr;
	Ground* m_Ground = nullptr;
	uint32_t m_TerrainCount = 0;
	math::float3 m_TerrainOffset = math::float3(0.0f, -15.0f, 1000.0f); // 地形のオフセット
	std::unordered_map<uint32_t, GameObject*> m_TerrainMap;
	std::list<std::wstring> m_PlayerBullets;
	std::list<std::wstring> m_PlayerMissiles;
	std::list<std::wstring> m_EnemyBullets;
	std::list<std::wstring> m_EnemyMissiles;
	std::list<std::wstring> m_Walls;
	std::list<std::wstring> m_Grounds;
};
