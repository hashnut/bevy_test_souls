#pragma once

#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"

#include "Camera/Define/GsCameraGameDefine.h"

//--------------------------------------
// 고정형 카메라
//--------------------------------------

class	FGsCameraModeHandler;

struct	FGsSchemaCameraModeData;

class FGsCameraModeFixedQuarter final : public FGsCameraModeControlBase
{
	using Super = FGsCameraModeControlBase;

	// 생성, 소멸자
public:
	FGsCameraModeFixedQuarter();
	virtual ~FGsCameraModeFixedQuarter() = default;

	// 가상 함수
public:
	virtual void Initialize() override;
	// 다음 스텝 진행
	virtual void NextStep(FGsCameraModeHandler* In_handler) override;
	

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// 인풋에 의한 모드 변환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;

	// 그리팅 끝 호출
	virtual void OnGreetingOff(bool In_isDirect = false) override;

	// 줌인
	virtual void ZoomIn(float In_mulVal) override;
	// 줌아웃
	virtual void ZoomOut(float In_mulVal) override;

	// 가상함수 get, set
public:
	// 입력 회전 사용하는지(Yaw)
	virtual bool IsUseInputRotationYaw()override { return false; }
	
	// 모드 회전 구하기
	virtual FRotator GetModeRot(const FGsSchemaCameraModeData* In_modeData) override;

	// 현재 줌min 체크 하는가
	virtual bool GetIsNowCheckZoomMin() override;


	// logic func
public:
	void ChangeYawMode();

	// 로직 함수 get, set
public:
	// 모드 회전 세팅 하기
	void SetModeRot(const FGsSchemaCameraModeData* In_modeData, bool In_isDirect = false);
};