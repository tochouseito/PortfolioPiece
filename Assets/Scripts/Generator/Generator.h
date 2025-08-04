#pragma once
#include "Marionnette.h"
#include <list>
#include <string>
#include "../BulletType.h"

// 前方宣言
class PlayerBullet;
class PlayerMissile;
class EnemyBullet;
class EnemyMissile;
class Wall;
class Ground;

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
	void GeneratePlayerBullet(const PlayerBulletType& type, const Vector3& pos);
	// プレイヤー弾削除
	void RemovePlayerBullet(const std::wstring& name);
private:
	PlayerBullet* m_PlayerBullet = nullptr;
	PlayerMissile* m_PlayerMissile = nullptr;
	EnemyBullet* m_EnemyBullet = nullptr;
	EnemyMissile* m_EnemyMissile = nullptr;
	Wall* m_Wall = nullptr;
	Ground* m_Ground = nullptr;
	std::list<std::wstring> m_PlayerBullets;
	std::list<std::wstring> m_PlayerMissiles;
	std::list<std::wstring> m_EnemyBullets;
	std::list<std::wstring> m_EnemyMissiles;
	std::list<std::wstring> m_Walls;
	std::list<std::wstring> m_Grounds;
};