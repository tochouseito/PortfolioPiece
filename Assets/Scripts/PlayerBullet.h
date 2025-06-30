#pragma once
#include "IScript.h"

class PlayerBullet : public IScript
{
public:
	// コンストラクタ
    PlayerBullet(GameObject& object) : IScript(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;

	void SetActive(bool isActive) { m_IsActive = isActive; } // アクティブ状態を設定
	bool IsActive() const { return m_IsActive; } // アクティブ状態を取得
private:
	GameObject* m_Player = nullptr; // プレイヤーオブジェクトへのポインタ

	float m_Speed = 400.0f; // 弾の移動速度
    Vector3 m_Direction{};// 進行方向
	bool m_IsActive = false; // アクティブ状態かどうか
};