#include "PlayerBullet.h"
using namespace ChoSystem;

void PlayerBullet::Start()
{
    // 初期化処理
    m_Player = &FindGameObjectByName(L"Player");
    gameObject.transform.scale().Initialize();


}

void PlayerBullet::Update()
{
    // 毎フレーム処理
    // テスト
	gameObject.transform.position().z += 10.1f;
    if(gameObject.transform.position().z > 1000.0f) {
        DestroyGameObject(gameObject.GetID());
        return;
	}
}

REGISTER_SCRIPT_FACTORY(PlayerBullet);