#pragma once
#include "GameObject/Camera/Work/Location/GsCameraLocationGame.h"

/*
	카메라 이동 대사연출 처리
*/

class FGsCameraLocationDialog : public FGsCameraLocationGame
{
	using Super = FGsCameraLocationGame;


	// 가상 함수
public:
	virtual void OnFinishLerp() override;

	// 회전속도 구하기
	virtual float GetRotSpeed()override;

	// 업데이트
	virtual void Update(float In_deltaTime) override;


};