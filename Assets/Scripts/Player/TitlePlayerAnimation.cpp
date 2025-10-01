#include "TitlePlayerAnimation.h"
using namespace theatriaSystem;

namespace
{
    constexpr float kTwoPi = 6.283185307179586f;
}

void TitlePlayerAnimation::Start()
{
    // 初期化処理
	m_BasePos = transform->position; // 今の位置を基準にする
	m_Phase = 0.0f;
}

void TitlePlayerAnimation::Update()
{
    // 毎フレーム処理
    // 位相を進める（角速度 = 2πf）
    m_Phase += kTwoPi * m_FrequencyHz * DeltaTime();
    if (m_Phase > kTwoPi)
    {
        m_Phase = std::fmod(m_Phase, kTwoPi); // オーバーフロー防止
    }

    // y をサインで上下させる（x,z は固定）
    Vector3 pos = m_BasePos;
    pos.y += m_Amplitude * std::sin(m_Phase);
    transform->position = pos;
	// スペースキーが押されたらシーン切り替え
	if (Input::TriggerKey(DIK_SPACE))
	{
		sceneManager.LoadScene(L"MainScene");
	}
}

REGISTER_SCRIPT_FACTORY(TitlePlayerAnimation);