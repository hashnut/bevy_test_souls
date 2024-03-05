#pragma once

#include "GameObject/Camera/Work/FOV/GsCameraFOVGame.h"

class FGsCameraFOVDialog : public FGsCameraFOVGame
{
	using Super = FGsCameraFOVGame;

	// 가상 함수
public:
	// 회전 속도 구하기
	virtual float GetRotSpeed()override;
	virtual void SetNowSpeed(float In_speed) override;
};