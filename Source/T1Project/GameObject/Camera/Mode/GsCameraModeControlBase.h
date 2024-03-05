#pragma once


#include "GameObject/Camera/Mode/GsCameraModeBase.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Camera/Define/GsCameraGameDefine.h"
#include "Message/GsMessageInput.h"

//--------------------------------------
// 스프링 암 사용한 것(백뷰, 쿼터 뷰)
//--------------------------------------

class	UGsGameObjectLocalPlayer;
class	AGsCharacterLocalPlayer;

class	FGsCameraOffsetGame;
class	FGsCameraTargetOffsetGame;
class	FGsCameraZoomGame;
class	FGsCameraRotationQuarter;

class FGsInputEventMsgBase;

struct	FGsSchemaCameraModeData;

class FGsCameraModeControlBase :public FGsCameraModeBase
{
	using Super = FGsCameraModeBase;
protected:
	// 현재 터치중인지
	bool _bTouchOn = false;


private:
	TArray<TPair<MessageInput, FDelegateHandle>>		_arrayInputMessageDelegates;

	// 생성자, 소멸자
public:
	FGsCameraModeControlBase() = default;
	FGsCameraModeControlBase(EGsCameraMode In_mode) : FGsCameraModeBase(In_mode) {}
	virtual ~FGsCameraModeControlBase() = default;

	// 가상함수
public:
	virtual void Enter() override;
	virtual void Exit() override;

	// 엔터에서 
	// 기본데이터 초기화와(InitMode)
	// 입력에 의한 연출(ChangeModeByInput) 분리
	// 그리팅(Greeting) 처리때문에 분리

	// 모드 초기화(데이터들)
	virtual void InitMode(
		AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData) override;
	// 처음 한번만 하는 초기화
	virtual void Initialize() override;

	// 터치 시작
	virtual void TouchPress();
	// 터치 끝
	virtual void TouchRelease(bool In_isSimpleTouch);
	// 그리팅 끝 호출
	virtual void OnGreetingOff(bool In_isDirect = false) override;
	// 줌인
	virtual void ZoomIn(float In_mulVal) override;
	// 줌아웃
	virtual void ZoomOut(float In_mulVal) override;
	// 그리팅 뷰 세팅
	virtual void SetGreetingView() override;

	// 일반적인 모드 전환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;

	virtual void ChangeModeDialogEnd() override;

	// 다른맵 스폰 초기화
	virtual void InitOtherMapSpawn(const FGsSchemaCameraModeData* In_modeData) override;
	// 같은맵 스폰 초기화
	virtual void InitSameMapSpawn(const FGsSchemaCameraModeData* In_modeData) override;
	// 보간 끝
	virtual void OnFinishLerpMode()override;

	// 모드줌 시작
	virtual void StartModeZoom(EGsCameraChangeZoomType In_type = EGsCameraChangeZoomType::ModeChange);

	// fov 변환 시작
	virtual void StartModeFOV() override;

	// zoom 에의한 fov 변경
	virtual void OnReachZoomFOV(bool In_isReached) override;

	virtual void ChangeMode(const FGsSchemaCameraModeData* In_modeData) override;
	virtual void Close() override;

	virtual void PressJoystick() {}

	// 줌 종료 콜백
	virtual void OnZoomFinished() override;

	// 가상 get,set
public:
	// 현재 줌값 구하기
	virtual float GetCurrentZoom() override;
	// 현재 회전 구하기(백업시 사용)
	virtual FQuat GetCurrentQuat() override;
	// 시작 모드 줌 값 얻어오기
	virtual float GetStartModeZoom();
	// 모드 회전 구하기
	virtual FRotator GetModeRot(const FGsSchemaCameraModeData* In_modeData);

	// message
public:
	void OnTouchPress(const FGsInputEventMsgBase& InMsg);
	void OnTouchRelease(const FGsInputEventMsgBase& InMsg);

	void OnPressJoystick(const FGsInputEventMsgBase& InMsg);


	// 로직함수
public:
	// 모드 소켓 오프셋 시작
	void StartModeSocketOffset(bool In_isDirect);
	void StartTargetSocketOffset(bool In_isDirect, FVector In_targetOffset);
	
	bool GetTouchOn() { return _bTouchOn; }
	// 카메라 offset 이동 (키, shift)
	void StartCameraOffset(bool In_isDirect);
	// 대화 연출 복구 완료 되었음
	void OnFinishDialogBackupLerp();

	void ReleaseMessageDelegate();

	void PredictTransform(FRotator& Out_rotation, FVector& Out_pos);

	void PostChangeModeNormal();

	void SetGreetingOffDof();
	void SetGreetingOffFov();

	// get, set
public:
	// 카메라 오프셋 처리 접근
	FGsCameraOffsetGame* GetCameraOffsetGame();

	FGsCameraTargetOffsetGame* GetCameraTargetOffsetGame();
	// 카메라 회전 처리 접근
	FGsCameraZoomGame* GetCameraZoomGame();

	// 카메라 회전 쿼터 처리 접근
	FGsCameraRotationQuarter* GetCameraRotationQuarter();
	// 그리팅 소켓 오프셋 계산
	void SetGreetingSocketOffest();
	// 현재 모드 fov(줌 체크)
	float GetNowModeFOV();
	// 모드 focal region 세팅
	void SetModeMobileFocalRegion(const FGsSchemaCameraModeData* In_modeData);
};