#pragma once
#include "IScript.h"

class MainCamera : public IScript
{
public:
	// コンストラクタ
    MainCamera(GameObject& object) : IScript(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
	// ラグ追従
	void LagFollow();

	GameObject* player = nullptr; // プレイヤーオブジェクトへのポインタ

	// オフセット
	const Vector3 offset = Vector3(0.0f, 5.0f, -30.0f); // オフセット
	const Vector3 lookOffset = Vector3(0.0f, 2.0f, 10.0f); // 視線の少し前
	const float followSpeed = 5.0f; // 追従速度
	const float rotateSpeed = 5.0f; // 回転速度
};