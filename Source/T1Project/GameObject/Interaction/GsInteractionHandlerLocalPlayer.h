#pragma once

#include "GameObject/Interaction/GsInteractionHandlerBase.h"
#include "GameObject/Interaction/GsReserveInteractionInfo.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "Message/GsMessageDelegateList.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

#include "Message/GsMessageInput.h"
#include "Message/GsMessageGameObject.h"

class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class UGsUIHUDQuestDistance;

struct FGsSchemaNpcData;
struct FGsSchemaPropData;
struct IGsMessageParam;

/*
 인터랙션 핸들러 로컬꺼
*/

class FGsInteractionHandlerLocalPlayer final : public FGsInteractionHandlerBase,
	public IGsReserveWork
{
	using Super = FGsInteractionHandlerBase;

private:
	// 멤버 변수
	// 예약 인터랙션 정보
	FGsReserveInteractionInfo _reserveInteractionInfo;
	TWeakObjectPtr<UGsGameObjectLocalPlayer> _local;
	// 경과 시간 체크
	FGsInteractionTimer _timer;

	// 메세지 델리게이트
	TArray<TPair<MessageInput, FDelegateHandle>> _actionDelegates;
	MsgGameObjHandleArray _gameobjectDelegates;

	MsgGameObjHandle _autoMoveRequestedDelegate;

	bool _isTestShowInteractionSphere = false;

	bool _isAutoMoveRequest = false;

	// 소멸자
public:
	virtual ~FGsInteractionHandlerLocalPlayer() = default;

	// static
public:
	// 인터랙션 범위 구하기
	//static float GetInteractionRange(const FGsSchemaNpcData* In_npcData, bool In_isOrigin = false);
	static float GetInteractionRange(const FGsSchemaPropData* In_propData, bool In_isOrigin = false);
	static float GetInteractionRange(const FGsSchemaNpcData* In_npcData, UGsGameObjectBase* In_Target = nullptr, bool In_isOrigin = false);

	static FVector GetShiftTargetNonplayerPos(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target, float In_acceptanceRaidus,
		bool In_isTestDrawLine);

	static bool GetShiftTargetNonplayerPosAndAcceptanceRaidus(
		UGsGameObjectBase* In_localGameObject,
		UGsGameObjectBase* In_targetGameObject,
		float In_acceptanceRadius,
		FVector& Out_newShiftPos, float& Out_newRadius);
	 

	// 가상함수
public:
	// 초기화
	virtual void Initialize(UGsGameObjectBase* In_owner) override;
	virtual void Update(float In_delta);
	virtual void Finalize() override;

	virtual void ClearTarget() override;

	// 예약 데이터 있는가
	virtual bool IsReserveData() override;
	// 재시도 해라
	virtual void DoRetry() override;

	// 예약된 데이터 클리어해라
	virtual void ClearReserveData() override;

public:	
	// 인터랙션 시작( 서버에서 내려줄때 처리)
	virtual void StartInteraction(UGsGameObjectBase* In_taret) override; 
	virtual void EndInteraction(UGsGameObjectBase* In_taret) override;
	virtual void CancelInteration(UGsGameObjectBase* In_taret) override;
	virtual void CancelInteration() override;

	// message bind
public:
	void OnLocalAutoMoveRequested(const IGsMessageParam* In_isMoveSuccess);
	void OnLocalAutoMoveIgnored(const IGsMessageParam*);
	// 자동 이동 컨텐츠 타입 변경됨
	// https://jira.com2us.com/jira/browse/C2URWQ-3193
	void OnAutoMoveContentsTypeChanged(const IGsMessageParam* In_param);

	// 로직 함수
public:
	// 인터랙션 시도(Npc 타겟)
	void TryInteractionNpcData(UGsGameObjectBase* In_target, const FGsSchemaNpcData* In_npcData);
	// 인터랙션 시도(Prop 타겟)
	void TryInteractionPropData(UGsGameObjectBase* In_target, const FGsSchemaPropData* In_propData);
	// 인터랙션 시도(위치)
	void TryInteractionPos(EGsGameObjectType In_type, int In_tblId, int In_mapId, FVector In_pos, float In_acceptanceRadius =0.f);
	// 인터랙션 이동(타겟)
	void OnMoveToInteractionGameObject(UGsGameObjectBase* In_target);
	// 인터랙션 이동(위치)
	void OnMoveToInteractionPos(int In_mapId, FVector In_targetPos, float In_addAcceptanceRadius = 0.f);
	// 이동 성공
	void CallbackMoveToInteractionEnd();
	// 이동 실패
	void CallbackMoveToInteractionFailed(const FPathFollowingResult& Result);
	// 예약된 데이터 클리어
	void ClearReserveInteraction();
	// 예약된게 있나
	bool IsReserveInteraction();

	// npc tbl id로 인터랙션
	void TryInteractionNpcTblId(int In_npcTblId, 
		EGsInteractionMoveType In_interactionType = EGsInteractionMoveType::None);

	// 4번째 인자 위치는
	// quest 대화 연출 이동시 다른맵으로 갈때
	// 해당 위치를 월드맵에서 목적지로 표시한다
	// https://jira.com2us.com/jira/browse/C2URWQ-5545
	void TryInteractionNpcTblIdWithMapId(int In_npcTblId, int In_mapId,
		EGsInteractionMoveType In_interactionType = EGsInteractionMoveType::None, 
		FVector In_otherMapRegionShowPos = FVector::ZeroVector);
	// 자동 진행 퀘스트 인터랙션
	void TryInteractionAIQuest(int In_npcId, int In_mapId,
		const FVector& In_pos, bool In_isCenterNode);

	void BindAutoMoveMessageByNPCFunctionSlot();
	// bind된 npc function slot 델리게이트 삭제
	void ClearBindAUtoMoveMessageByNPCFunctionSlot();

	// 예약 데이터 삭제와 auto move stop으로 상태 전환
	// 스킬에서 특정 이동일때만 처리하는거 때문에 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	void ClearReserveDataAndAutoMoveStop();

public:
	// 경과 시간 설정
	void StartTimer(float In_EndTime);
	double GetElapsed() const;
	void UpdateTimer(float In_Delta);

private:
	// 인터랙션 시도 내부 사용
	void TryInteractionInternal(UGsGameObjectBase* In_target);
	// 인터랙션 데이터 설정 공통 함수
	void TryInteraction(EGsGameObjectType In_type, UGsGameObjectBase* In_target, EGsInteractionMoveType In_moveType,
		int In_tblId = 0, int In_mapId = 0, FVector In_pos = FVector::ZeroVector);

	void CallbackTargetClearObject(const IGsMessageParam* inParam);
	// Destroy 대상 체크
	void CallbackCheckDestroyObject(const IGsMessageParam* inParam);
	// 키입력 이벤트 체크
	void CallbackPressJoystick(const FGsInputEventMsgBase& In_msg);

	// 스킬 버튼 누름
	void OnPressSkill(const FGsInputEventMsgBase& In_msg);
	// 스킬 클릭
	void OnClickSkill(const FGsInputEventMsgBase& In_msg);
	// 1차 타겟
	void OnLocalTargetChanged(const IGsMessageParam* In_param);
	// 로컬 스폰 완료 시점
	void OnLocalSpawnMeComplete(const IGsMessageParam*);

	bool IsInsideInteractionRange();

	void ShowNpcListMoveInfo();
	void HideNpcListMoveInfo();

	// get, set
public:
	EGsInteractionMoveType GetReserveMoveType() { return _reserveInteractionInfo._moveType; }
	int GetReserveMapId() { return _reserveInteractionInfo._mapId; }
	FVector GetReserveMovePos() { return _reserveInteractionInfo._pos; }
	float GetReserveAddAcceptanceRadius() { return _reserveInteractionInfo._addAcceptanceRadius; }

	const FGsReserveInteractionInfo& GetReserveInfo() const { return _reserveInteractionInfo; }
	const FGsInteractionTimer& GetTimerInfo() const { return _timer; }
	float GetRatio() { return GetTimerInfo()._currTime / GetTimerInfo()._endTime;}

	// npc list 이동 거리
	bool GetNpcListMoveTargetDist(OUT float& Out_remainDist);

	UGsUIHUDQuestDistance* GetDistanceHud();

	void SetIsTestShowInteractionSphere(bool In_val)
	{
		_isTestShowInteractionSphere = In_val;
	}

	bool GetIsTestShowInteractionSphere()
	{
		return _isTestShowInteractionSphere;
	}

	float GetInteractionRangeReserveNpcData();

public:
	virtual bool IsInteraction() const override;
};