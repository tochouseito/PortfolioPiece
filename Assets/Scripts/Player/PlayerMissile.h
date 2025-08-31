#pragma once
#include "Marionnette.h"

// 前方宣言
class Player;
class Generator;
class Enemy;

class PlayerMissile : public Marionnette
{
public:
	// コンストラクタ
    PlayerMissile(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// 衝突処理
	void OnCollisionEnter(GameObject& other) override;
    // アクティブフラグ設定
	void SetActive(bool isActive) { m_IsActive = isActive; }
	// ターゲットの敵設定
	void SetTargetEnemy(Enemy* enemy) { m_TargetEnemy = enemy; }
private:
	// ホーミング
	void Homing();
	// 消滅
	void Remove();

	Player* m_Player = nullptr;// プレイヤーオブジェクトへのポインタ
	Generator* m_Generator = nullptr;// 弾生成器
	Enemy* m_TargetEnemy = nullptr;// ターゲットの敵

	// パラメータ
    bool m_IsActive = false;
	Vector3 m_Direction = Vector3(0.0f, 0.0f, 1.0f);// 進行方向
	Vector3 m_Angle = Vector3::Zero();// 角度
	Vector3 m_Velocity = Vector3::Zero();// 速度
	Vector3 m_AngleVelocity = Vector3::Zero();// 角速度
	float m_LifeTime = 60.0f;
	float m_MaxSpeed = 230.0f;// 移動速度
	float m_AngleSpeed = 1.5f;// 角速度
	float m_ElapsedTime = 0.0f; // 経過時間
	float m_AccelDuration = 1.5f; // 加速時間
	float m_CurrentSpeed = 0.0f; // 現在の速度
};