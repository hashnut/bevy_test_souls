// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/GsInputBindingBase.h"
#include "Input/GsInputEventValidator.h"
#include "TouchMove/GsReserveTouchMoveInfo.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageDelegateList.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageGameObject.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/EGsPartsType.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsInputBindingLocalPlayer.generated.h"


class FGsJoystickInputProcessor;
class FGsInputKeyEventValidator;
class FGsHerculesInputProcess;

class AActor;

struct IGsMessageParam;


// hit result 정렬 정보
struct FGsHitResultSortInfo
{
	// 거리 값(로컬 중심)
	float _distSquared;
	// 정렬 대상
	AActor* _actor;
	// 충돌 채널
	ECollisionChannel _collisionChannel;
	// 디버깅용 액터 위치
	FVector _actorPos;
 };


/**
* 내 Player 키입력 관련 바인딩 클래스
*/

UCLASS()
class T1PROJECT_API UGsInputBindingLocalPlayer : 
	public UGsInputBindingBase,
	public IGsReserveWork,
	public TGsMessageDelegateList<MInput, MessageInput>
{
	GENERATED_BODY()
	
	DECLARE_DELEGATE_OneParam(FOnAttack1, int32);
	DECLARE_DELEGATE_OneParam(FOnAttachParts, EGsPartsType);

	DECLARE_DELEGATE_TwoParams(FOnOptionKeyMapping, int32, TEnumAsByte<EInputEvent>);

	// 멤버 변수들
protected:
	class UGsGameObjectLocalPlayer* _owner;
	FVector _axisValue = FVector::ZeroVector;
	float _axisYForwardValue;
	float _axisYBackwardValue;
	float _axisXRightValue;
	float _axisXLeftValue;

	FVector _prevAxisValue = FVector::ZeroVector;
	float _touchMoveFactorMobile = 0.25f; // 터치 이동거리 조절값(값이 작아질수록 움직임 작아짐)(모바일)
	float _touchMoveFactorPC = 0.25f; // 터치 이동거리 조절값(값이 작아질수록 움직임 작아짐)(PC)

	float _touchMoveLimit = 1.5f; // 터치 이동거리 제한값(min, max가 됨)
	bool _bTouchPressed1 = false; // 1번 손가락 터치 했는가
	bool _bTouchPressed2 = false; // 2번 손가락 터치 했는가
	FDateTime _touchPressTime;

	TSharedPtr<FGsJoystickInputProcessor> _joystickInputProcessor;
	TSharedPtr<FGsHerculesInputProcess> _heculesInputProcessor;


private:
	// 터치 줌인, 아웃용
	// 0번 터치 이전 위치
	FVector _prevTouchLoc1;
	// 1번 터치 이전 위치
	FVector _prevTouchLoc2;
	// 누른 위치
	FVector2D _pressLoc2D;
	// 이전 거리
	float _prevDist;
	// 터치줌 조절값
	float _touchZoomFactor = 0.5;

	// pitch 회전에 곱할 비율값
	float _rotPitchAspectRate = 0.0f;
	// pitch 회전 추가 곱
	float _pitchRotMul = 1.0f;

	/** _blockInputFlag 는 현재 Flag 값을 의미.입력 막는 케이스를 Flag 로 관리할 것 */
	EGsKeyMappingInputFlags _blockInputFlag = EGsKeyMappingInputFlags::INPUT_NONE;

	/** 스택관리하려 했으나, 이벤트가 열고 닫히는 순서 보장이 되지 않아, Set 으로 관리 */
	TSet<EGsKeyMappingInputFlags> _blockInputFlagSet;

	// Key Input 과 현재 State 에 따라 허용되는지 여부 관리
	TSharedRef<FGsInputKeyEventValidator> _inputKeyEventValidator;

	bool _isZoomMode = false;

	// 테스트 오토 인풋 타임 핸들
	FTimerHandle _testAutoInputTimeHandle;
	// 테스트 오토 인풋 타임 on
	bool _isTestAutoInputTimerOn = false;
	// 테스트 터치 관련 출력 할지
	bool _isTestPrintTouch = false;
#if UE_EDITOR
	int _testTempValue = 0;
#endif
	// 터치 시작인지
	bool _isTouchStart = false;
	// 디버그용 터치 이동 그릴지
	bool _isShowDebugTouchMove = false;
	// draw debug arrow
	bool _isDrawDebugArrow = false;

	UPROPERTY()
	UClass* _touchMovePosClass = nullptr;

	// 터치 이동 위치 액터
	UPROPERTY()
	AActor* _touchMovePosActor = nullptr;

	// gameobject 관련 메시지 핸들러
	MsgGameObjHandleArray _gameObjectMsgDelegates;
	// system msg delegate
	TArray<TPair<MessageSystem, FDelegateHandle>> _systemMsgDelegates;
	// 터치 이동 예약 정보
	FGsReserveTouchMoveInfo _reserveTouchMoveInfo;
	// 인터랙션할 충돌 채널들
	TArray<ECollisionChannel> _arrayChannelInteraction;
	// 인터랙션은 안하지만 터치 이동은 못하게 하는 충돌체(ex: PawnBossBlock 시공의 틈새 오염체 )
	TArray<ECollisionChannel> _arrayChannelNonTouchMoveTarget;

	// 조이스틱 누르고 있는가
	bool _isJoystickPressed = false;
	bool _isJoystickClicked = false;
	FDateTime _joystickPressTime;

public:
	UGsInputBindingLocalPlayer();

	// 각종 가상 함수들
public:
	virtual void Initialize() override;
	virtual void Initialize(class UGsGameObjectLocalPlayer* target);
	virtual void Update(float Delta) override;
	virtual void Finalize() override;

	virtual void SetBinding(UInputComponent* input) override;

	// 예약된 데이터 클리어해라
	virtual void ClearReserveData() override;

	// logic 함수들
protected:
	//Local Character Bind
	// void OnAction();
	void OnMoveForward(float Value);
	void OnTestMoveForward(int32 Value);
	void OnMoveBackward(float Value);
	void OnMoveLeft(float Value);
	void OnMoveRight(float Value);
	void OnMoveRotateYaw(float Value);
	void OnMoveRotatePitch(float Value);

	// Touch Interaction
	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 1
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 0
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location); // Location Z == 1

	void OnTouchJoystickPress();
	void OnTouchJoystickRelease();

	// 줌인
	void OnZoomIn();
	// 줌아웃
	void OnZoomOut();
	// 카메로 모드 변경
	void OnViewChange();

	// 테스트 오토 체인지
	void OnTestAutoChange();
	// 테스트 오토 체인지 타이머 
	void OnToggleTestAutoChangeTimer();
	// 테스트 포션 이펙트 출력
	void OnTestPostionEffect();
	// 테스트 디버프 액티브
	void OnTestDebuffActive();
	// 테스트 디버프 디액티브
	void OnTestDebuffDeactive();
	// 입력 막기 이벤트 (플래그 기반)
	void OnBlockInputFlag(const class FGsInputEventMsgBase& InMsg);

	void OnBackKey();
	// 단순 터치 처리(이동 x)
	void OnSimpleTouch(FVector In_touchLocation);

	// Message Bind
	void OnPressSkill(const class FGsInputEventMsgBase& InMsg);
	void OnReleaseSkill(const class FGsInputEventMsgBase& InMsg);
	void OnClickSkill(const class FGsInputEventMsgBase& InMsg);
	void OnUnmountVehicle(const class FGsInputEventMsgBase& InMsg);	
	void OnViewChange(const class FGsInputEventMsgBase& InMsg);	
	void OnTestPrintTouch(const class FGsInputEventMsgBase& InMsg);	

	// 1차 타겟
	void OnLocalTargetChanged(const IGsMessageParam* In_param);
	
	// 조이스틱 놓는 순간 호출
	void OnReleaseJoystick();
	// 조이스틱 누르는 순간 호출
	void OnPressJoystick();

	// 줌모드 갱신
	void UpdateZoomMode();	

	void OnQuickExecute();
	// https://jira.com2us.com/jira/browse/C2URWQ-5354
	// 플레이어블 이벤트 평타 스페이스바 인풋 받기
	void OnPlayableEventNoramlAttack();

protected:
	void OnKeyboardSkill(int32 InKeyMappingType, EInputEvent InEventType);

	// 현재는 사용하지 않음
	// void OnInputPressAxisSkill();
	// void OnInputReleaseAxisSkill();

	void OnKeyboardActionOptionKey(int32 InKeyMappingType, TEnumAsByte<EInputEvent> InInputEventType);

	// 지형 터치 처리
	void ProcTouchTerrain(const FVector& In_touchLocation);
	// 터치 이동 액터 만들기
	void MakeTouchMoveActor(const FVector& In_pos);

	void BindMessage();
	void ReleaseMessage();
public:
	// 터치 이동 액터 삭제
	void DeleteTouchMoveActor();
	// 로컬 죽을때
	void OnLocalDie();

	// 이동 성공
	void CallbackTouchMoveEnd();
	// 이동 실패
	void CallbackTouchMoveFailed(const struct FPathFollowingResult& Result);

	// 예약된 터치 이동 진행
	void ProcessReserveTouchMove();
	// 예약된 터치 이동 정보 클리어
	void ClearReserveTouchMoveData();
	// 지형 터치 이동 시작
	void StartTerrainTouchMove(const FVector& In_pos);

	// 인터랙션 액터 클릭(other collision: 타유저, pawn: npc, 몬스터, 프랍등)
	bool TouchInteractionActor(const FVector2D& In_touchPos, class IGsActorInteractionInterface** Out_interactionInterface);
	// 터치 이동 못하게 하는 충돌체(터치 이벤트를 그냥 소모하게 한다)
	bool TouchNonTouchMoveActor(const FVector2D& In_touchPos);
	// 예약 데이터 삭제와 auto move stop으로 상태 전환
	// 스킬에서 특정 이동일때만 처리하는거 때문에 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	void ClearReserveDataAndAutoMoveStop();

	// msg bind
public:
	void OnSpectatorTerrainTouchEffect(const struct IGsMessageParam* InParam);
	void OnLocalPlayerDie(const struct IGsMessageParam*);
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);
	// load to po start
	void OnLoadTopoStart(const IGsMessageParam* inParam);

public:
	// 데이터 예약이 있는가
	virtual bool IsReserveData() override;
	// 재시도 해라
	virtual void DoRetry() override;

	// Getter/Setter
public: 
	void SetTouchMoveFactor(float In_val);
	float GetTouchMoveFactor();
	void SetTouchMoveLimit(float In_val)
	{
		_touchMoveLimit = In_val;
	}
	float GetTouchMoveLimitMax()
	{
		return _touchMoveLimit;
	}
	float GetTouchMoveLimitMin()
	{
		return -_touchMoveLimit;
	}
	void SetTouchZoomFactor(float In_val)
	{
		_touchZoomFactor = In_val;
	}
	float GetTouchZoomFactor()
	{
		return _touchZoomFactor;
	}

	/** 
	* 입력 막기 처리. 윈도우, 튜토리얼 등의 상황에서 인풋을 막거나 해제 메시지를 보낼 때마다 호출되는 함수이다.
	*
	* @InFlag : SETTING_MOVE 같은 InputFlag 를 Set 에 추가하거나 지울 것이다.
	* @InIsAdd : true 이면 추가, false 이면 삭제이다.
	* @InFlushAll : true 일 경우, 기존에 쌓여 있는 블락 Flag 를 전부 없애고 리셋한다(죽음/스폰의 경우 적용).
	*/ 
	void SetIsBlockInputFlag(EGsKeyMappingInputFlags InFlag, bool InIsAdd = true, bool InFlushAll = false);

	void SetPitchRotMul(float In_val)
	{
		_pitchRotMul = In_val;
	}

	void SetIsShowDebugTouchMove(bool In_val)
	{
		_isShowDebugTouchMove = In_val;
	}

	void SetIsDrawDebugArrow(bool In_val)
	{
		_isDrawDebugArrow = In_val;
	}

	bool GetIsDrawDebugArrow()
	{
		return _isDrawDebugArrow;
	}

	// 컨텐츠 상태가 local 터치 사용하나
	// 뽑기 제외
	bool IsLocalTouchUseContentsGameState();
	// 터치 이동 예약된게 있나
	bool IsReserveTouchMoveData();
	// 지형 터치 이동 범위안인가
	bool IsInsideTerrainTouchMovePosRange(const FVector& In_targetPos);

	bool GetTerrainPosByScreenPos(const FVector2D& In_screenPos, FVector& Out_terrainPos);


	//
	void OnOptionKeyMapping(UInputComponent* input);

	bool IsInputBlock(EGsKeyMappingType In_Key = EGsKeyMappingType::NONE);
	bool IsInputFlagBlock(EGsKeyMappingInputFlags In_Key = EGsKeyMappingInputFlags::INPUT_NONE);
};
