#include "BlockGenerator.h"
using namespace ChoSystem;

void BlockGenerator::Start()
{
    // 初期化処理
    GameObject* block = FindGameObjectByName(L"block");
    if (block)
    {
        // X
        for (uint32_t i = 0; i < m_BlockCount; ++i)
        {
            // Y
            for (uint32_t j = 0; j < m_BlockCount; ++j)
            {
                // Z
                for(uint32_t k = 0; k < m_BlockCount; ++k)
                {
					Vector3 position = Vector3(i * m_BlockOffset.x, j * m_BlockOffset.y, k * m_BlockOffset.z);
                    GameObject* newBlock = CloneGameObject(block, position);
                    m_BlockNames.push_back(newBlock->GetName());
				}
            }
        }
    }
}

void BlockGenerator::Update()
{
    // 毎フレーム処理
}

REGISTER_SCRIPT_FACTORY(BlockGenerator);