#pragma once
#include <Marionnette.h>

// 前方宣言
class Player;

// 照準レティクルUIを表示するクラス
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
    math::float3 offset = math::float3(0.0f, 0.0f, 40.0f);
};
