#pragma once
#include "CoreMinimal.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBase.h"
#include "Camera/Define/GsCameraGameDefine.h"

/*
	카메라 인게임 쿼터 회전 처리
*/

class	AGsCharacterLocalPlayer;

class FGsCameraRotationQuarter : public FGsCameraRotationBase
{
	using Super = FGsCameraRotationBase;
private:
	// 줌 회전 하는지
	// 이값을 IsZoomStart와 따로 사용하는 이유는
	// 처음에 모드 전환시
	// 백뷰는 괜찮지만
	// 쿼드는 모드전환 회전과
	// 줌인 회전이 같이 일어나므로
	// 따로 처리한다
	bool _isUsingZoomRot = false;

	// 가상 함수
public:
	virtual void InitMode() override;
	virtual void SetRotation(const FRotator& In_rot);
	virtual bool IsRotationPossible() override;
	virtual void OnFinishLerp() override;
	// 회전속도 구하기
	virtual float GetRotSpeed()override;
	// 속도 만들기
	virtual void MakeSpeed()override;
	// get, set
public:
	void SetIsUsingZoomRot(bool In_val) { _isUsingZoomRot = In_val; }
};