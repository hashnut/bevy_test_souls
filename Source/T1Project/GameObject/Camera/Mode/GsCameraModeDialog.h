#pragma once

#include "GameObject/Camera/Mode/GsCameraModeBase.h"
/*
 카메라 모드: 대화 연출
*/

class	AGsCharacterLocalPlayer;
class	FGsCameraLocationDialog;
class	FGsCameraRotationDialog;
class	FGsCameraRotationDialogCutLoop;
class	FGsCameraZoomDialog;
class	UCameraComponent;
struct FGsSchemaCameraModeData;

struct FGsCameraDOFSetting
{
	bool bOverride_DepthOfFieldFstop;
	float DepthOfFieldFstop;
	bool bOverride_DepthOfFieldFocalDistance;
	float DepthOfFieldFocalDistance;

	// Mobile
	bool bOverride_DepthOfFieldFocalRegion;
	float DepthOfFieldFocalRegion;
	float DepthOfFieldScale;
};


class FGsCameraModeDialog final : public FGsCameraModeBase
{
	using Super = FGsCameraModeBase;

private:
	// 더티 방식으로 다음틱에 view 세팅 처리 하기
	// actor 회전 처리 한뒤 바로하면 생기는 문제
	// 1. npc greeting 할 때 옆으로 시도하면 옆면을 그리팅 할때 있음
	// 2. 카메라 바로 이동 세팅시 카메라가 엉뚱한데로 감
	bool _isDirtyNextTickViewSet = false;
	const FGsSchemaCameraModeData* _dirtyModeData;

	FGsCameraDOFSetting _backUpSetting;

	bool _isDirtyFinishLerpMode = false;

	FRotator _directRotToBackup = FRotator::ZeroRotator;

	bool _isUseDirectRotToBackup = false;
private:
	UCameraComponent* GetCameraComponent();
	void SetupCameraDOF();
	float GetFocaDistance();
#if WITH_EDITOR
	bool IsPCPlatform();
#endif
    
public:
	FGsCameraModeDialog();
	// 소멸자
	virtual ~FGsCameraModeDialog() = default;

	// 각종 가상 함수들
public:
	virtual void Initialize() override;

	virtual void Enter() override;
	virtual void Exit() override;

	// 모드 변환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;
	// 보간 끝
	virtual void OnFinishLerpMode()override;

	// fov 변환 시작(대사는 모드fov가 아니라 view 데이터 참조)
	virtual void StartModeFOV() override;

	// pitch min max 초기화
	virtual void InitPitchMinMax(APlayerController* In_playerController,
		const FGsSchemaCameraModeData* In_modeData) override;

	virtual void Update(float In_deltaTime) override;

	// 모드 초기화(데이터들)
	virtual void InitMode(
		AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData) override;

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// 로직 함수
public:
	// 모드 회전 저장 하기
	void SetModeRot(const FGsSchemaCameraModeData* In_modeData);

	// 모드 뷰 변경
	void ChangeModeView();
	// 뷰 데이터 FOV로 변경
	void StartViewDataFOV(bool In_isDirect);
	// 다음틱 뷰 세팅
	void NextTickViewSet();

	void SetDialogCutLoop();

	// get
public:
	// 카메라 위치 처리 접근(대사)
	FGsCameraLocationDialog* GetCameraLocationDialog();
	// 카메라 회전 처리 접근(대사)
	FGsCameraRotationDialog* GetCameraRotationDialog();
	// camera zoom (dialog)
	FGsCameraZoomDialog* GetCameraZoomDialog();
	// camera cut loop (dialog)
	FGsCameraRotationDialogCutLoop* GetCameraDialogCutLoop();

};