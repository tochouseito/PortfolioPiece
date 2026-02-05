#pragma once
#include "BaseCamera.h"

// TPS視点のカメラクラス
class TPSCamera : public BaseCamera
{
public:
	TPSCamera();
	~TPSCamera();
	void Start() override;
	void Update() override;
private:

};
