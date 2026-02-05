#pragma once
#include "Marionnette.h"

// 地面オブジェクトを表すクラス
class Ground : public Marionnette
{
public:
	// コンストラクタ
    Ground(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;

	void SetScale(const Scale& scale);
};
