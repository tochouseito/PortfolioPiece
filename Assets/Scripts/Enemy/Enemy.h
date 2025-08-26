#pragma once
#include "Marionnette.h"

// 前方宣言
class EnemySpawner;

class Enemy : public Marionnette
{
public:
	// コンストラクタ
    Enemy(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// アクティブ設定
	void SetActive(bool isActive) { m_IsActive = isActive; }
	// 衝突関数
	void OnCollisionEnter(GameObject& other) override;
private:
	EnemySpawner* m_EnemySpawner = nullptr;

    bool m_IsActive = false;
};