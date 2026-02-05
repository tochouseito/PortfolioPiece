#include "Ground.h"
using namespace theatriaSystem;

// Start の処理
void Ground::Start()
{
    // 初期化処理
}

// Update の処理
void Ground::Update()
{
    // 毎フレーム処理
}

// SetScale の処理
void Ground::SetScale(const Scale& scale) { transform->scale = scale; }

REGISTER_SCRIPT_FACTORY(Ground);
