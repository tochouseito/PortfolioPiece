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
private:
	GameObject* m_Player = nullptr; // プレイヤーオブジェクトへのポインタ

    Vector3 m_Direction{};// 進行方向
};