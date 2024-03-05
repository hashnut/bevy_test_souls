#pragma once

#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"

/*
	워프 시작 모드(줌 아웃)
*/
struct	FGsSchemaCameraModeData;

class FGsCameraRotationWarpStart;

class FGsCameraModeWarpStart final : public FGsCameraModeControlBase
{
	using Super = FGsCameraModeControlBase;
	// 생성
public:
	FGsCameraModeWarpStart();

	// 가상함수
public:
	// 처음 한번만 하는 초기화
	virtual void Initialize() override;
	// 모드 변환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;

	// 모드줌 시작
	virtual void StartModeZoom(EGsCameraChangeZoomType In_type = EGsCameraChangeZoomType::ModeChange)override;

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// get, set
public:
	FGsCameraRotationWarpStart* GetCameraRotationWarpStart();
};