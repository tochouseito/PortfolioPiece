#include "Target.h"
using namespace theatriaSystem;
#include "Player/Player.h"
#include "Camera/MainCamera.h"
#include "Enemy/EnemySpawner.h"
#include "Enemy/Enemy.h"
#include "LockOn.h"

// Start の処理
void Target::Start()
{
    // 初期化処理
    m_Player = GetMarionnette<Player>(L"Player");
	m_MainCamera = GetMarionnette<MainCamera>(L"MainCamera");
	m_EnemySpawner = GetMarionnette<EnemySpawner>(L"EnemySpawner");
	m_LockOn = GetMarionnette<LockOn>(L"LockOn");
    transform->position = offset;
    transform->parent = m_Player->gameObject.GetHandle().entity;
}

// Update の処理
void Target::Update()
{
    // 毎フレーム処理
    EnemyLockOn();
}

// リストから削除
void Target::RemoveLockOn(const std::wstring& name)
{
	m_LockOnMap.erase(name);
}

// EnemyLockOn の処理
void Target::EnemyLockOn()
{
    // ロックオン処理
	Camera camera = m_MainCamera->GetComponent<Camera>();
    math::float2 screenPos = math::WorldToScreen(math::GetTranslation(transform->matWorld), camera->viewMatrix, camera->projectionMatrix, static_cast<uint32_t>(ScreenWidth()), static_cast<uint32_t>(ScreenHeight()));
	// 敵のリストを取得
	auto enemyMap = m_EnemySpawner->GetEnemyMap();
    // ロックオン
    for (auto& pair : enemyMap)
    {
        Enemy* enemy = pair.second;
        if (!enemy->IsActive()) continue;// 非アクティブはスルー
        if (enemy->IsLockOnTarget()) continue;// ロックオンされている敵はスルー
		if (math::GetTranslation(enemy->transform->matWorld).IsZero()) continue;// 座標がゼロベクトルはスルー
		Vector2 enemyScreenPos = math::WorldToScreen(math::GetTranslation(enemy->transform->matWorld), camera->viewMatrix, camera->projectionMatrix, static_cast<uint32_t>(ScreenWidth()), static_cast<uint32_t>(ScreenHeight()));
        float distance = (screenPos - enemyScreenPos).Length();
        // if の処理
        if (distance < lockOnRadius)
        {
            // ロックオン成功
            // ロックオンレティクル生成
			GameObject* dst = CloneGameObject(&m_LockOn->gameObject, float3());
			LockOn* lockOn = dst->GetMarionnette<LockOn>();
			lockOn->SetTargetEnemy(enemy);
			// リストに追加
			m_LockOnMap[dst->GetName()] = lockOn;
            enemy->EnableLockOnTarget(lockOn);
			// 親子の設定
			lockOn->transform->parent = enemy->gameObject.GetHandle().entity;
        }
    }
}

REGISTER_SCRIPT_FACTORY(Target);
