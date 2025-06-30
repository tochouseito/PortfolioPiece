#pragma once
#include "IScript.h"
#include "BulletType.h"
#include <list>
#include <string>

class PlayerBulletGenerator : public IScript
{
public:
	// コンストラクタ
    PlayerBulletGenerator(GameObject& object) : IScript(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
    // 弾生成
	void GenerateBullet(const PlayerBulletType& type, const uint32_t& count);
private:
	GameObject* m_Player = nullptr; // プレイヤーのGameObject
	GameObject* m_PlayerBullet = nullptr; // プレイヤー弾のGameObject

	std::list<std::wstring> m_PlayerBulletList; // プレイヤー弾のリスト
};