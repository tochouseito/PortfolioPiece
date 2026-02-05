#pragma once
#include <Marionnette.h>

// 敵弾の移動と衝突処理を行うクラス
class EnemyBullet : public Marionnette
{
public:
	// コンストラクタ
    EnemyBullet(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;

	void OnCollisionEnter(GameObject& other) override;

	// アクティブ状態を設定
	void SetActive(bool isActive) { m_IsActive = isActive; }
	// 進行方向を設定
	void SetDirection(const math::float3& dir) { m_Direction = dir; }
	// ダメージ量を取得
	int GetDamage() const { return m_Damage; }
private:
	class Generator* m_Generator = nullptr;

	bool m_IsActive = false;
	float m_LifeTime = 60.0f * 4.0f;
	float m_Speed = 180.0f;
	math::float3 m_Direction = math::float3(0.0f, 0.0f, 1.0f);
	math::float3 m_Velocity = math::float3::Zero();
	int m_Damage = 1;
};
