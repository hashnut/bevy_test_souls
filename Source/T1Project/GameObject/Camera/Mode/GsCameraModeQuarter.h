#pragma once

#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"

//----------------------------------
// follow Quarter 모드
//----------------------------------

class	FGsCameraModeHandler;

struct	FGsSchemaCameraModeData;

class FGsCameraModeQuarter final :public FGsCameraModeControlBase
{
	using Super = FGsCameraModeControlBase;
	// 생성, 소멸자
public:
	FGsCameraModeQuarter();
	virtual ~FGsCameraModeQuarter() = default;

	// 가상함수
public:
	virtual void Initialize() override;
	// 다음 스텝 진행
	virtual void NextStep(FGsCameraModeHandler* In_handler) override;
	
	// 줌인
	virtual void ZoomIn(float In_mulVal) override;
	// 줌아웃
	virtual void ZoomOut(float In_mulVal) override;

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// 인풋에 의한 모드 변환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;

	// 그리팅 끝 호출
	virtual void OnGreetingOff(bool In_isDirect = false) override;

	// 가상 get, set
public:
	// 현재 줌min 체크 하는가
	virtual bool GetIsNowCheckZoomMin() override;


	// 로직 함수
public:
	// 모드 회전 세팅 하기
	void SetModeRot(const FGsSchemaCameraModeData* In_modeData, bool In_isDirect = false);
};