#pragma once

#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"

//------------------------------
// 시공의 틈새 카메라 모드
// 쿼터와 비슷하고 테이블 값이 다르다
//------------------------------

class FGsCameraModeSpaceCrack final : public FGsCameraModeControlBase
{
	using Super = FGsCameraModeControlBase;

	// 생성, 소멸자
public:
	FGsCameraModeSpaceCrack();
	virtual ~FGsCameraModeSpaceCrack() = default;

	// 가상함수
public:
	virtual void Initialize() override;

	// 줌인
	virtual void ZoomIn(float In_mulVal) override;
	// 줌아웃
	virtual void ZoomOut(float In_mulVal) override;

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// 가상 get, set
public:
	// 시작 모드 줌 값 얻어오기
	virtual float GetStartModeZoom() override;
	// 모드 회전 구하기
	virtual FRotator GetModeRot(const FGsSchemaCameraModeData* In_modeData) override;

	// 현재 줌min 체크 하는가
	virtual bool GetIsNowCheckZoomMin() override { return true; }
};

