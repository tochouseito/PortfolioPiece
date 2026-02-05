#pragma once
#include "Marionnette.h"

// 前方宣言
class Enemy;

// ロックオンUIの対象管理クラス
class LockOn : public Marionnette
{
public:
	// コンストラクタ
    LockOn(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
	// ターゲットの敵設定
	void SetTargetEnemy(Enemy* enemy) { m_TargetEnemy = enemy; }
	// ターゲットの敵取得
	Enemy* GetTargetEnemy() const { return m_TargetEnemy; }
	// 攻撃されたかどうか設定
	void SetIsAttacked(bool attacked) { isAttacked = attacked; }
	// 攻撃されたかどうか取得
	bool GetIsAttacked() const { return isAttacked; }
private:
	Enemy* m_TargetEnemy = nullptr;// ターゲットの敵

	bool isAttacked = false;// 攻撃されたかどうか
};
