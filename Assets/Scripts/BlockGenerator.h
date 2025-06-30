#pragma once
#include "IScript.h"
#include <list>
#include <string>

class BlockGenerator : public IScript
{
public:
	// コンストラクタ
    BlockGenerator(GameObject& object) : IScript(object) {}
    // 初期化処理
    void Start() override;
	// 毎フレーム処理
    void Update() override;
private:
	std::list<std::wstring> m_BlockNames; // ブロック名のリスト
	uint32_t m_BlockCount = 10; // ブロックの数
	Vector3 m_BlockOffset = { 10.0f,10.0f ,10.0f }; // ブロックのオフセット
};