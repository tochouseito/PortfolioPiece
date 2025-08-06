#pragma once
#include "Marionnette.h"

// 前方宣言
class Player;
class Generator;

class PlayerBullet : public Marionnette
{
public:
	// コンストラクタ
    PlayerBullet(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;

	// 衝突時の処理
	void OnCollisionEnter(GameObject& other) override;

	void SetActive(bool isActive) { m_IsActive = isActive; } // アクティブ状態を設定
	bool IsActive() const { return m_IsActive; } // アクティブ状態を取得
private:
	Player* m_Player = nullptr; // プレイヤーオブジェクトへのポインタ
	Generator* m_Generator = nullptr;// 弾生成器

	float m_LifeTime = 60.0f * 6.0f;
	float m_Speed = 4.0f; // 弾の移動速度
    Vector3 m_Direction{};// 進行方向
	bool m_IsActive = false; // アクティブ状態かどうか
};