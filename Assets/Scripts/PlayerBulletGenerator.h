#pragma once
#include "Marionnette.h"
#include "BulletType.h"
#include <list>
#include <string>

class PlayerBulletGenerator : public Marionnette
{
public:
	// コンストラクタ
    PlayerBulletGenerator(GameObject& object) : Marionnette(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
    // 弾生成
	void GenerateBullet(const PlayerBulletType& type, const uint32_t& count);
	// リストから弾を削除
	void RemoveBulletFromList(const std::wstring& bulletName);
private:
	GameObject* m_Player = nullptr; // プレイヤーのGameObject
	GameObject* m_PlayerBullet = nullptr; // プレイヤー弾のGameObject

	std::list<std::wstring> m_PlayerBulletList; // プレイヤー弾のリスト
};