#pragma once

#include "Classes/GsState.h"
#include "Classes/GsStateMng.h"
#include "Classes/Container/GsSharedMap.h"

#include "GameObject/Camera/Mode/GsCameraModeBase.h"
#include "GameObject/GsGameObjectHandler.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsCameraMessageParam.h"

#include "Camera/Define/GsCameraGameDefine.h"

//--------------------------------------------------------------
// 카메라 모드 할당관리자
//--------------------------------------------------------------

class FGsCameraModeAllocator : TGsSharedMapAllocator<EGsCameraMode, FGsCameraModeBase>
{
public:
	FGsCameraModeAllocator() {}
	virtual ~FGsCameraModeAllocator() {}
	virtual FGsCameraModeBase* Alloc(EGsCameraMode In_mode) override;
};


/*
 카메라 모드 핸들러
*/

class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;
class FGsInputEventMsgBase;


struct IGsMessageParam;

class FGsCameraModeHandler : public IGsGameObjectHandler,  public TGsStateMng<FGsCameraModeBase, FGsCameraModeAllocator>
{
	// 타입 별칭 선언
	using Super = TGsStateMng<FGsCameraModeBase, FGsCameraModeAllocator>;

	// 멤버 변수
private:
	// 캐릭터 초기화 했는지?
	bool _isInitCharacter = false;

	// 로컬 플레이어
	UGsGameObjectLocalPlayer* _localPlayer = nullptr;


	// 카메라 대화 메시지 델리게이트
	MsgGameObjHandleArray _arrCameraDialogDelegate;
	// system 메세지 해제용
	MsgSystemHandleArray _systemMsg;
	// 액션 메시지 해제용
	TArray<TPair<MessageInput, FDelegateHandle>> _actionDelegates;
	TArray<TPair<MessageContentCamera, FDelegateHandle>> _msgContentCameraDelegates;
	// 로컬 원래 회전값
	FRotator _backupLocalRot;
	// 대사 대상 object
	// 여러명이면 끝에 2명이 들어감
	TArray<UGsGameObjectBase*> _arrDialogTargetObj;
	// 스폰할 때 초기화 하는 모드 변환인지
	bool _isSpawnInitModeChange = true;
	// spot id 위치 사용하는지(한번이라도)
	bool _isUsedSpotIdPos = false;
	// 로컬 저장 위치
	FVector _backupLocalPos;

	// 조이스틱 눌렸는가
	bool _isJoystickPressed = false;


	bool _isDirtyStartDialog = false;
	int _currentDirectGreetingNextTickCount = 0;
	int _directGreetingCheckNextTickCount = 0;
	FGsCameraDialogMessageParam _camDialogMessage;

	TFunction<void()> _callbackWarp;
	bool _isDirtyCallbackWarp = false;


	EGsCameraMode _backupBeforeEquipView;

	// 가상 함수
public:
	FGsCameraModeHandler() : IGsGameObjectHandler(GameObjectHandlerType::CAMERA) {}
	virtual ~FGsCameraModeHandler() = default;

public:

	virtual void ChangeState(TStateType In_state) override;
	
public:
	// 초기화(캐릭터 스폰할때 처리)
	virtual void Initialize(UGsGameObjectBase* In_local) override;
	virtual void InitializeActor(class AActor* Owner) override;
	// 해제
	virtual void Finalize() override;

	virtual void Update(float In_deltaTime) override;

public:
	// 컨트롤러 possess이후 호출됨
	void OnGreeting();
	// 다음 스텝으로 진행
	void NextStep();
	// 메시지 초기화
	void InitializeMessage();
	// 메시지 클리어
	void FinalizeMessage();

	// 대사 연출 관련
	// 대화 연출 시작
	void StartDialog();
	// 대화 연출 끝
	void EndDialog();
	// 로컬이랑 타겟 서로 바라보게 처리
	void StartLookatTarget(TArray<UGsGameObjectBase*> In_dialogTargets);

	void SaveCurrentLocalRotation();
	
	// 로컬 원래 회전으로 돌아가기
	void EndLookatTarget();
	void SetAttachFollowCamera(bool In_isAttach);

	void SetAttachFollowCameraToController(bool In_isAttach);
	// 대화 모드 뷰 변경(컷당 변화)
	void ChangeDialogModeView(TArray<UGsGameObjectBase*> In_dialogTargets);

	// mode랑 work 타겟 obj 변경
	void ChangeModeTargetObj(UGsGameObjectBase* In_target, bool Is_firstTime);

	void ChangeFixedQuarterYawMode();

	void GreetingOffDirect();

	void PostDirectGreetingDialogStart();

	void OnWarp(TFunction<void()> In_callbackWarp);

	void ProcWarp();

	void SetStartCameraDataTransform();
	void StartPlayableEventStartCam();
	void OnFinishPlayableCamLerp(bool In_isStartCam);

	void StartPlayableEventEndCam();
	// B1:https://jira.com2us.com/wiki/pages/viewpage.action?pageId=329686045 
	// himichani
	// set local player pos quest dialog target 
	void SetLocalPosTargetFront(UGsGameObjectBase* In_target);

	void SetLocalPlayerPos(FVector In_pos);

	void SetLocalPosition(int In_spotId, TArray<UGsGameObjectBase*> In_arrDialogTargetObj);

	void SendEnvStartDialog();
	void SendEnvEndDialog();

	void ClearEquipView();

	void CloseEquipUI();

	
	// 이벤트
public:
	// 카메라 대화 연출 시작
	void OnCameraDialogStart(const IGsMessageParam* In_param);
	// 카메라 대화 연출 끝
	void OnCameraDialogEnd(const IGsMessageParam*);
	// 대사 백업값으로 보간 끝
	void OnFinishDialogBackupLerp();
	// 카메라 대화 모드 뷰 변경
	void OnCameraDialogModeViewChange(const IGsMessageParam* In_param);
	// 워프전 처리
	void OnLoadTopoStart(const IGsMessageParam* inParam);

	// 조이스틱 땜
	void OnReleaseJoystick(const FGsInputEventMsgBase&);
	// 조이스틱 누름
	void OnPressJoystick(const FGsInputEventMsgBase&);
	// fixed quarter mode yaw type change(north, east, south, west)
	void OnClickFixedQuarterYaw();

	void OnCameraEquipViewStart(const IGsMessageParam*);
	void OnCameraEquipViewEnd(const IGsMessageParam*);

	void OnLocalWarp(const IGsMessageParam* In_param);

	// get, set
public:
	TSharedRef<FGsCameraModeBase>	GetState()
	{
		return CurrentState.Pin().ToSharedRef();
	}

	// 대화 대상 구하기
	TArray<UGsGameObjectBase*> GetDialogTargets() { return _arrDialogTargetObj; }
	// 초기화 모드 타입 구하기
	EGsCameraInitModeType GetInitModeType();
	void SetDialogTarget(TArray<UGsGameObjectBase*> In_targets) { _arrDialogTargetObj = In_targets; }

	// 로컬이 spot id pos 사용하는지
	void SetLocalUseSpotIdPos(bool In_isUse);
	// 로컬 위치 변경(spot id 위치 이동)
	void SetLocalPosSpotId(int64 In_spotId);
	// 로컬 위치 변경()
	void SetLocalPosBackup();

	bool GetPressJoystick()
	{
		return _isJoystickPressed;
	}

};