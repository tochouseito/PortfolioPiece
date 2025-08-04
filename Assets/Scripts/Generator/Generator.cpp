#include "Generator.h"
using namespace ChoSystem;
#include "Player/PlayerBullet.h"
#include "Player/PlayerMissile.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/EnemyMissile.h"
#include "FieldObject/Ground.h"
#include "FieldObject/Wall.h"

void Generator::Start()
{
    // 初期化処理
	m_PlayerBullet = GetMarionnette<PlayerBullet>(L"PlayerBullet");
	m_PlayerMissile = GetMarionnette<PlayerMissile>(L"PlayerMissile");
	m_EnemyBullet = GetMarionnette<EnemyBullet>(L"EnemyBullet");
	//m_EnemyMissile = GetMarionnette<EnemyMissile>(L"EnemyMissile");
	m_Wall = GetMarionnette<Wall>(L"Wall");
	m_Ground = GetMarionnette<Ground>(L"Ground");
}

void Generator::Update()
{
    // 毎フレーム処理
}

void Generator::GeneratePlayerBullet(const PlayerBulletType& type, const Vector3& pos)
{
	GameObject* dst = CloneGameObject(&m_PlayerBullet->gameObject, pos);
	PlayerBullet* bullet = dst->
	m_PlayerBullets.push_back(dst->GetName());
}

void Generator::RemovePlayerBullet(const std::wstring& name)
{
	m_PlayerBullets.remove(name);
}

REGISTER_SCRIPT_FACTORY(Generator);