#pragma once
#include "Marionnette.h"

class Player;
class Reticle : public Marionnette
{
public:
	// コンストラクタ
    Reticle(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
    // プレイヤー
    Player* m_Player = nullptr;

    // パラメータ
    Vector3 offset = Vector3(0.0f, 0.0f, 40.0f);
};