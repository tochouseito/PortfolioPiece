#include "PlayerMissile.h"
using namespace ChoSystem;
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Generator/Generator.h"

void PlayerMissile::Start()
{
    // 初期化処理
	m_Player = GetMarionnette<Player>(L"Player");
	m_Generator = GetMarionnette<Generator>(L"Generator");
	if (!m_IsActive) { return; }
    m_Direction.Normalize();
    m_Velocity.Initialize();
    m_Angle = transform->degrees;
    transform->quaternion = m_Player->transform->quaternion;
	m_LifeTime = 60.0f * 6.0f; // ライフタイムを設定

	// 姿勢をターゲットに向ける
    Vector3 enemyPos = m_TargetEnemy->transform->position;
	Vector3 toEnemy = enemyPos - transform->position;
	// y軸回りの回転（ヨー）
    m_Angle.y = std::atan2(toEnemy.x, toEnemy.z);
	// y軸周りに-θy回す回転行列を作成
	Matrix4 rotY = chomath::MakeRotateYMatrix(m_Angle.y);
	// toEnemyに回転行列を掛け算してvelocityZを求める
    Vector3 velocityZ = chomath::Multiply(toEnemy, -rotY);
	// x軸回りの回転（ピッチ）
    m_Angle.x = std::atan2(-velocityZ.y, velocityZ.z);
}

void PlayerMissile::Update()
{
    // 毎フレーム処理
	if (!m_IsActive) return;
	// ライフタイムが0以下なら消滅
    if (m_LifeTime <= 0.0f)
    {
        // ライフタイムが0以下なら非アクティブにする
        Remove();
    }
	// ホーミング処理
	Homing();
    // ライフタイム更新
    m_LifeTime--;
    Rigidbody3D rb = GetComponent<Rigidbody3D>();
    rb->velocity = m_Velocity; // Rigidbody3Dの速度を更新
    rb.SetTransform(rb->quaternion);
	//rb->angularVelocity = m_AngleVelocity; // Rigidbody3Dの角速度を更新
}

// 衝突処理
void PlayerMissile::OnCollisionEnter(GameObject& other)
{
    // otherがEnemyなら
    if (other.GetTag() == "Enemy")
    {
        Remove();
    }
}

void PlayerMissile::Homing()
{
    // 移動
	Vector3 enemyPos = m_TargetEnemy->transform->position;
	Vector3 toEnemy = enemyPos - transform->position;
	toEnemy.Normalize();
    m_Direction = toEnemy;
    m_Velocity = chomath::Slerp(m_Velocity, m_Direction, 0.2f);
    m_Velocity.Normalize();
	m_Velocity *= m_Speed;

 //   // 回転
	//// y軸回りの回転（ヨー）
 //   m_Angle.y = std::atan2(m_Velocity.x, m_Velocity.z);
	//// y軸周りに-θy回す回転行列を作成
	//Matrix4 rotY = chomath::MakeRotateYMatrix(m_Angle.y);
 //   // velocityに回転行列を掛け算してvelocityZを求める
	//Vector3 velocityZ = chomath::Multiply(m_Velocity, -rotY);
	//// x軸回りの回転（ピッチ）
 //   m_Angle.x = std::atan2(-velocityZ.y, velocityZ.z);

    // 回転姿勢をターゲット方向に向ける
    Quaternion targetRot = chomath::MakeLookRotation(m_Velocity, Vector3(0.0f,1.0f,0.0f));

    Rigidbody3D rb = GetComponent<Rigidbody3D>();
    // 現在の回転から目標回転へ補間
	rb->quaternion = Quaternion::Slerp(rb->quaternion, targetRot, 0.2f);

	//// 角度を徐々に近づける
 //   m_AngleVelocity = chomath::Slerp(m_AngleVelocity, m_Angle, 0.2f);
	//m_AngleVelocity.Normalize();
 //   m_AngleVelocity *= m_AngleSpeed;
}

void PlayerMissile::Remove()
{
	// ミサイルを非アクティブにする
    m_IsActive = false;
    // 生成器から削除
    if (m_Generator)
    {
        m_Generator->RemovePlayerBullet(gameObject.GetName());
    }
    DestroyGameObject(&gameObject);
}

REGISTER_SCRIPT_FACTORY(PlayerMissile);