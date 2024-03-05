#pragma once

#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"

//-----------------------------------------
// 플레이어블 이벤트 용 카메라()
//----------------------------------------

class	FGsCameraModeHandler;
class  AGsCharacterLocalPlayer;
class FGsCameraLocationGame;
class FGsCameraRotationBase;

struct	FGsSchemaCameraModeData;

class FGsCameraModePlayableEvent final : public FGsCameraModeControlBase
{
	using Super = FGsCameraModeControlBase;

private:
	bool _isStartCamLerp = true;

	// 생성, 소멸자
public:
	FGsCameraModePlayableEvent();
	virtual ~FGsCameraModePlayableEvent() = default;

	// 가상 함수
public:
	virtual void Initialize() override;

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// 줌인
	virtual void ZoomIn(float In_mulVal) override;
	// 줌아웃
	virtual void ZoomOut(float In_mulVal) override;

	virtual void InitMode(
		AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData) override;

	// 다른맵 스폰 초기화
	virtual void InitOtherMapSpawn(const FGsSchemaCameraModeData* In_modeData) override;
	// 보간 끝
	virtual void OnFinishLerpMode()override;
	// logic func
public:

	void SetStartCameraDataTransform();
	void StartPlayableEventStartCam();
	void StartPlayableEventEndCam();

	void MakePlayableEventCamLerpSpeed(bool In_isStartCam,
		FGsCameraLocationGame* In_locationWork, FGsCameraRotationBase* In_rotationWork);

	// 가상함수 get, set
public:

	// 모드 회전 구하기
	virtual FRotator GetModeRot(const FGsSchemaCameraModeData* In_modeData) override;

	

};