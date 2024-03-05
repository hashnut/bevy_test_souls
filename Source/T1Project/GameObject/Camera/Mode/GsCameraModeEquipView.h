#pragma once
// camera mode: equip view

#include "GameObject/Camera/Mode/GsCameraModeBase.h"
#include "Camera/Define/GsCameraGameDefine.h"

struct FGsSchemaCameraModeData;

class FGsCameraZoomGame;
class FGsCameraTargetOffsetGame;
class FGsCameraRotationBackView;
class FGsCameraOffsetGame;

class FGsCameraModeEquipView final : public FGsCameraModeBase 
{
	using Super = FGsCameraModeBase;

public:
	FGsCameraModeEquipView();
	virtual ~FGsCameraModeEquipView() = default;

	// virtual func
public:
	virtual void Initialize() override;
	// 모드 변환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;

	// fov 변환 시작
	virtual void StartModeFOV() override;

	// 업데이트
	virtual void Update(float In_deltaTime) override;

	// logic func
public:
	// 모드줌 시작
	void StartModeZoom(const FGsSchemaCameraModeData* In_modeData,
		EGsCameraChangeZoomType In_type = EGsCameraChangeZoomType::ModeChange);

	void StartCameraOffset();

	void StartModeSocketOffset();

	void StartTargetSocketOffset(FVector In_targetOffset);
	// follow 갱신
	void UpdateFollow(float In_deltaTime);
	// 따라가기 시작
	void StartFollow(EGsCameraInitType In_initType);

	void StartDOFChange();

	//get, set func
public:
	// 카메라 회전 처리 접근
	FGsCameraZoomGame* GetCameraZoomGame();
	FGsCameraTargetOffsetGame* GetCameraTargetOffsetGame();
	// 카메라 회전 백뷰 처리 접근
	FGsCameraRotationBackView* GetCameraRotationBackView();
	// 카메라 오프셋 처리 접근
	FGsCameraOffsetGame* GetCameraOffsetGame();
	
};