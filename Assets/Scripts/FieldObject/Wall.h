#pragma once
#include <Marionnette.h>

// 壁オブジェクトを表すクラス
class Wall : public Marionnette
{
public:
	// コンストラクタ
    Wall(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
};
