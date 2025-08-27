#pragma once
#include "Marionnette.h"
#include <list>

// 前方宣言
class Player;
class MainCamera;
class EnemySpawner;
class LockOn;

class Target : public Marionnette
{
public:
	// コンストラクタ
    Target(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// ロックオンリスト取得
	std::list<std::wstring>& GetLockOnList() { return m_LockOnList; }
	// リストから削除
	void RemoveLockOnList(const std::wstring& name);
private:
    void EnemyLockOn();

	Player* m_Player = nullptr;// プレイヤー
	MainCamera* m_MainCamera = nullptr;// メインカメラ
	EnemySpawner* m_EnemySpawner = nullptr;// エネミースポナー
	LockOn* m_LockOn = nullptr;// ロックオン
	std::list<std::wstring> m_LockOnList;// ロックオンリスト

    // パラメータ
    Vector3 offset = Vector3(0.0f, 0.0f, 40.0f);
	float lockOnRadius = 120.0f;// ロックオン半径
};