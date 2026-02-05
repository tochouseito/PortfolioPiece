#pragma once
#include "Marionnette.h"

// 敵ミサイルのホーミング挙動を行うクラス
class EnemyMissile : public Marionnette
{
public:
	// コンストラクタ
    EnemyMissile(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;

	void OnCollisionEnter(GameObject& other) override;

	// アクティブ状態を設定
	void SetActive(bool isActive) { m_IsActive = isActive; }
	// ダメージ量を取得
	int GetDamage() const { return m_Damage; }
private:
	void Homing();
	void Remove();

	class Player* m_Player = nullptr;
	class Generator* m_Generator = nullptr;

	bool m_IsActive = false;
	math::float3 m_Direction = math::float3(0.0f, 0.0f, 1.0f);
	math::float3 m_Velocity = math::float3::Zero();
	float m_LifeTime = 60.0f * 6.0f;
	float m_Speed = 140.0f;
	float m_HomingStrength = 0.08f;
	int m_Damage = 1;
};
