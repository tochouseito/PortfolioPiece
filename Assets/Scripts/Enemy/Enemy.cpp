#include "Enemy.h"
using namespace ChoSystem;

void Enemy::Start()
{
    // 初期化処理
    // タグ設定
	gameObject.SetTag("Enemy");
}

void Enemy::Update()
{
    // 毎フレーム処理
}

REGISTER_SCRIPT_FACTORY(Enemy);