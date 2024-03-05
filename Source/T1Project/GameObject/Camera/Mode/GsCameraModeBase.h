#pragma once

#include "DataSchema/GsSchemaEnums.h"

#include "Classes/GsState.h"

#include "Camera/Define/GsCameraGameDefine.h"

//----------------------------------------------------------------------
// 카메라 모드 기반
//----------------------------------------------------------------------

class	APlayerController;

class	FGsCameraModeManager;
class	UGsGameObjectLocalPlayer;
class	UGsGameObjectBase;
class	AGsCharacterLocalPlayer;

class	FGsCameraLocationGame;
class	FGsCameraRotationBase;
class	FGsCameraFOVGame;
class	FGsCameraDOFGame;
class	FGsCameraModeHandler;

class	IGsCameraWork;

struct	FGsSchemaCameraModeData;


class FGsCameraModeBase : 
	public TGsState<EGsCameraMode>
{
protected:
	// 로컬 플레이어
	UGsGameObjectLocalPlayer* _localPlayer;
	// 현재 카메라 붙어있는 타겟 플레이어(관전에서는 로컬이 아닐수 있다)
	UGsGameObjectBase* _targetPlayer;
	FGsCameraModeHandler* _handler;

	// 이전에 그리팅이었는지
	bool _isBeforeGreeting = false;
	// 카메라 기능들
	TMap<EGsCameraWorkType, IGsCameraWork*> _mapCamWorks;

	// 이동 보간 끝
	bool _locationLerpFinished = false;
	// 회전 보간 끝
	bool _rotaionLerpFinished = false;
	// 이동, 회전, fov 속도(대사 연출에서 같은 속도로 처리위한것)
	float _currentWorkSpeed = 0.0f;

	// 생성자
public:
	FGsCameraModeBase() : TGsState<EGsCameraMode>(EGsCameraMode::Quarter) {}
	FGsCameraModeBase(EGsCameraMode In_mode) :TGsState<EGsCameraMode>(In_mode) {}

	virtual ~FGsCameraModeBase() = default;

	// 가상함수
public:
	virtual void Enter() override ;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
	// 마지막 정리할때
	virtual void Close() override;

public:
	virtual void SetAxisValue(const FVector& inAxisValue) {};
	// 줌인
	virtual void ZoomIn(float In_mulVal) {};
	// 줌아웃
	virtual void ZoomOut(float In_mulVal) {}
	// 다음 스텝 진행(매니저 넘김)
	virtual void NextStep(FGsCameraModeHandler* In_handler);
	// 캐릭터가 이동 될때 여기가 호출된다(자동이동, 수동 이동)
	virtual void UpdateMove(bool In_isAuto);
	// 자동 공격시 회전 갱신
	virtual void UpdateAttack(bool In_isAuto);
	// 줌 종료 콜백
	virtual void OnZoomFinished() {}
	// 그리팅 끝 호출
	virtual void OnGreetingOff(bool In_isDirect = false);

	// 그리팅 호출 후처리
	virtual void OnGreetingOffAfter();

	

	// 엔터에서 
	// 기본데이터 초기화와(InitMode)
	// 입력에 의한 연출(ChangeModeByInput) 분리
	// 그리팅(Greeting) 처리때문에 분리

	
	// 모드 초기화(데이터들)
	virtual void InitMode(
		AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData);
	// pitch min max 초기화
	virtual void InitPitchMinMax(APlayerController* In_playerController, 
		const FGsSchemaCameraModeData* In_modeData);

	// 모드 변환
	virtual void ChangeMode(const FGsSchemaCameraModeData* In_modeData);
	// 다른맵 스폰 초기화
	virtual void InitOtherMapSpawn(const FGsSchemaCameraModeData* In_modeData) {}
	// 같은맵 스폰 초기화
	virtual void InitSameMapSpawn(const FGsSchemaCameraModeData* In_modeData) {}

	// 대사 모드 끝나고 처리
	virtual void ChangeModeDialogEnd() {}
	// 일반적인 모드 전환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData);

	// 캐릭터 삭제 처리
	virtual void RemoveCharacter();

	// 처음 한번만 하는 초기화
	virtual void Initialize();

	// 카메라 타겟 대상세팅
	void SetModeTarget(UGsGameObjectBase* In_target);
	// 로컬 플레이어 세팅
	void SetModeLocalObject(UGsGameObjectLocalPlayer* In_local);

	// 그리팅 뷰 세팅
	virtual void SetGreetingView() {}

	// 보간 끝
	virtual void OnFinishLerpType(EGsDialogCameraFinishType In_type);
	virtual void OnFinishLerpMode() {}
	// 현재 줌값 구하기
	virtual float GetCurrentZoom() { return 0.f; }
	// 현재 회전 구하기(백업시 사용)
	virtual FQuat GetCurrentQuat() { return FQuat::Identity; }
	
	// fov 변환 시작
	virtual void StartModeFOV() {}
	// zoom 에의한 fov 변경
	virtual void OnReachZoomFOV(bool In_isReached) {}

	// 가상 함수 get, set
public:
	// 입력 줌 사용하는지
	virtual bool IsUseInputZoom() { return true; }
	// 입력 회전 사용하는지(Yaw)
	virtual bool IsUseInputRotationYaw() { return true; }
	// 입력 회전 사용하는지(Pitch)
	virtual bool IsUseInputRotationPitch() { return true; }
	// 현재 줌min 체크 하는가
	virtual bool GetIsNowCheckZoomMin()
	{
		return false;
	}

public:
	// 그리팅 off 처리
	void GreetingOff(bool In_isDirect = false);

	// 줌 최소 도달 콜백
	void OnReachZoomMin(bool In_isReached);
	// make common speed(for sync)
	void MakeDialogSpeed(FGsCameraLocationGame* In_locationWork, FGsCameraRotationBase* In_rotationWork);

	void GreetingOffForeced();

	void CameraRotationChanged();
	void OnJoysticChanged();

	// get, set
public:
	
	void SetHandler(FGsCameraModeHandler* In_handler)		{ _handler = In_handler; }
	// 모드 회전 변환중인지
	bool GetIsModeRotChanging();

	// 카메라 기능 접근
	IGsCameraWork* GetCameraWork(EGsCameraWorkType In_type);

	// 카메라 위치 처리 접근
	FGsCameraLocationGame*	GetCameraLocationGame();
	// 카메라 회전 처리 접근
	FGsCameraRotationBase*	GetCameraRotationBase();
	// 카메라 fov 처리 접근
	FGsCameraFOVGame*		GetCameraFOVGame();
	// 카메라 dof 처리 접근
	FGsCameraDOFGame*		GetCameraDOFGame();

	bool GetIsLocationLerpFinished()
	{
		return _locationLerpFinished;
	}
	// 이전에 그리팅이었는지(현재 그리팅인지)
	bool GetIsBeforeGreeting()
	{
		return _isBeforeGreeting;
	}
	// now target
	// 1. local player
	// 2. remote(observer mode)
	UGsGameObjectBase* GetCameraTargetPlayer()
	{
		return _targetPlayer;
	}
};
