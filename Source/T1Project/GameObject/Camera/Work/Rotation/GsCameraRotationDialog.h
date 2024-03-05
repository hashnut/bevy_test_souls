#pragma once
#include "CoreMinimal.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBase.h"


/*
	카메라 회전 대화 처리: 
*/


class FGsCameraRotationDialog :public FGsCameraRotationBase
{
	using Super = FGsCameraRotationBase;

	// 가상함수
public:
	virtual void SetRotation(const FRotator& In_rot);
	virtual void OnFinishLerp()override;

	// 회전속도 구하기
	virtual float GetRotSpeed()override;


};
