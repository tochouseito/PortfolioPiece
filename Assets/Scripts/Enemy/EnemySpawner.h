#pragma once
#include "Marionnette.h"
#include <list>
#include <string>

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
private:
    void SpawnEnemy();

    Player* m_Player = nullptr;
    Enemy* m_Enemy = nullptr;
	std::list<std::wstring> m_Enemies;
};