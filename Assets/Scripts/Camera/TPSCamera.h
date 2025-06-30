#pragma once
#include "BaseCamera.h"

class TPSCamera : public BaseCamera
{
public:
	TPSCamera();
	~TPSCamera();
	void Start() override;
	void Update() override;
private:

};