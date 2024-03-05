// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "input/GsInputEventMsgBase.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/MessageParam/GsQuestMessageParam.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "GameObject/GsGameObjectHandler.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Currency/GsCostPackage.h"


class UGsGameObjectLocalPlayer;
class FGsQuestInstance;
class FGsQuestPoolManager;
class FGsQuestActionManagement;
class UGsUIPopupQuestRepeatAccept;
struct FGsSchemaRewardData;
struct FGsSchemaQuestGuideStory;

/**
 *  퀘스트 핸들러
 */
class T1PROJECT_API FGsQuestHandler : public IGsGameObjectHandler, public IGsEventProgressInterface
{
protected:
	UGsGameObjectLocalPlayer*	_owner = nullptr;					// LocalPlayer
	TArray<FGsQuestInstance*>	_questInstanceList;					// 현재 진행중인 퀘스트리스트			
	FGsQuestInstance*			_mainQuestInstance = nullptr;		// 현재 진행중인 메인퀘스트 (단일로 존재)	
	FGsQuestPoolManager*		_questPoolManager = nullptr;		// QuestInstance 풀 관리를 위한 매니저		

	TArray<TPair<MessageContentQuest, FDelegateHandle>>	_listQuestDelegate;
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_itemContentsDelegateList;
	TArray<TPair<MessageInput, FDelegateHandle>>		_actionDelegates;
	TPair<MessageUserInfo, FDelegateHandle>				_levelHandler;

	MsgGameObjHandleArray								_listGameObjectDelegate;	
	MsgSystemHandleArray								_listSystemDelegate;
	MsgUIHandleArray									_listUiDelegate;
	MsgUIHandle											_stopMovieHandle;

	// 퀘스트 이동 팝업 띄우는 예약 데이터(대사 연출과 거의 동시에 처리시 버그 이슈)
	TSharedRef<FGsCostPackage>	_reservedCostPackage = MakeShared<FGsCostPackage>();
	QuestObjectiveId			_reserveObjectiveId;
	QuestId						_reserveQuestId;
	QuestKey					_reserveQuestKey;
	bool						_reserveIsAccpeted = true;
	StoryId						_reserveStoryIdAccpet = 0;
	StoryId						_reserveStoryIdReward = 0;

	TArray<QuestKey>			_requestList;
	QuestKey					_refreshQuestKey;

	bool						_repeatWarpWaiting = false;				// 반복 퀘스트 자동 진행 대기중
	bool						_isWarpReady = false;					// 워프 대기상태
	bool						_isUseWarpRandomScroll = false;			// 랜덤 이동 주문서 사용 여부

public:
	FGsQuestHandler() : IGsGameObjectHandler(GameObjectHandlerType::QUEST) {};
	virtual ~FGsQuestHandler() = default;

public:
	virtual void Initialize(UGsGameObjectBase* Local) override;
	virtual void Finalize() override;

	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

public:
	FGsQuestInstance* FindQuestInstance(const QuestKey& inQuestKey, StoryId inStoryId = 0);			// 퀘스트 아이디로 QuestInstance 찾기

private:
	FGsQuestInstance* ClaimQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey);			// 퀘스트 QuestInstance 생성
	void FlushAllInstance();																		// 모든 QuestInstance 삭제
	void AcceptQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey, QuestState inState);
	void RemoveQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey);						// QuestId로 questInstanceList에서 삭제하고 풀에서도 삭제
	void RemoveQuestInstance(FGsQuestInstance* inQuest);											// FGsQuestInstance로 풀에서 삭제	
	void UpdateQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey, QuestState inState);	// 퀘스트 QuestInstance 업데이트
	void UpdateQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey, QuestState inState,
							const TArray<int32>& inObjectiveValueList, bool inIsUpdate, 
							EventActionIndex inPreEventActionIndex, EventActionIndex inPostEventActionIndex,
													EventActionIndex inPostStoryEventActionIndex);	// 퀘스트 QuestInstance 업데이트 (오브젝티브 까지)	

private:
	void OnQuestAutoMoveEnd(const IGsMessageParam* In_data);			// 퀘스트 자동 이동 완료
	void OnQuestAutoMoveNextMap(const IGsMessageParam* In_data);		// 퀘스트 자동 이동 중 맵이동 시 도착위치 알림
	void OnQuestAutoMoveWarpStart(const IGsMessageParam* In_data);		// 퀘스트 자동 이동 중 워프 시작 알림
	void OnInteractionNpcContents(const IGsMessageParam* In_data);		// npc Interaction을 했다.
	void OnInteractionPropStart(const IGsMessageParam* In_data);		// Prop 시작
	void OnInteractionPropEnd(const IGsMessageParam* In_data);			// Prop 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam* In_data);	// 로컬pc가 스폰이 끝나는 시점에 퀘스트 인스턴스의 생성
	void OnLocalPlayerSpawnAction(const IGsMessageParam* In_data);		// 로컬pc가 준비가 되면 퀘스트 인스턴스 업데이트 시작
	void OnUIAction(const FGsInputEventMsgBase& In_msg);				// 스킬, 조이스틱 인풋 메세지
	void OnUIActionHudAuto(const IGsMessageParam* In_data);				// hud 오토버튼
	void OnSequencePlayerStop(const IGsMessageParam*);					// 시네마틱이 끝나서 자동진행을 다시 요청한다.	
	void OnChangeLocalPlayerLevel(uint64 InLevel);						// user레벨이 변경 됐을 때
	void OnQuestActiveOff(const IGsMessageParam*);						// 퀘스트 액티브 끄기(다른 컨텐츠에서)
	void OnAutoMoveFailed(const IGsMessageParam* In_data);				// 자동이동 실패
	void OnAutoProgressRequest(const IGsMessageParam*);					// 퀘스트 자동진행 요청
	void OnScanClick(const IGsMessageParam*);							// 스캔사용
	void OnQuestWindowOpen(const IGsMessageParam*);						// 퀘스트 윈도우 창이 열렸을 때
	void OnLocalCampHit(const IGsMessageParam* In_data);				// 캠프와 충돌했을 때
	void OnLocalReaction(const IGsMessageParam* In_data);				// 로컬pc Reaction
	void OnWarpReady();													// 워프 대기
	void OnWarpFailure();												// 워프 실패

public:
	void AddQuest(QuestId inQuestId);												// 습득 메세지 (클라 -> 클라)
	void AddQuestSub(StoryId inStoryId, QuestId inQuestId);
	void AddQuestRepeat(StoryId inStoryId, QuestId inQuestId);
	void AddStoryQuest(QuestId inQuestId);											// 스토리의 첫번째 퀘스트 습득 메세지(클라 -> 클라)
	void AcceptQuest(const StoryId inStoryId, const QuestKey& inQuestKey);			// 수락 메세지 (서버 -> 클라)
	void AcceptQuestSub(const StoryId inStoryId, const QuestKey& inQuestKey);		// 수락 메세지 (서버 -> 클라)
	void AcceptQuestRepeat(const StoryId inStoryId, const QuestKey& inQuestKey);	// 수락 메세지 (서버 -> 클라)
	void AcceptQuestGuide(const QuestKey& inQuestKey);								// 수락 메세지 (서버 -> 클라)
	void AcceptQuestBoard(const StoryId inStoryId, const QuestKey& inQuestKey);		// 수락 메세지 (서버 -> 클라)

	void UpdateQuest(const StoryId inStoryId, const QuestKey& inQuestKey,
						const QuestState inState, const TArray<int32>& inObjectiveValueList, bool inIsUpdate,
							EventActionIndex inPreEventActionIndex, EventActionIndex inPostEventActionIndex, 
									EventActionIndex inPostStoryEventActionIndex);		// 업데이트 메세지 (서버 -> 클라)
	void RewardQuest(const StoryId inStoryId, const QuestId inQuestId);					// 메인 보상 메세지 (서버 -> 클라)
	void RewardQuestSub(const StoryId inStoryId, const QuestId inQuestId);				// 서브 보상 메세지 (서버 -> 클라)
	void RewardQuestRepeat(const StoryId inStoryId, const QuestKey& inQuestKey);		// 반복 보상 메세지 (서버 -> 클라)
	void RewardQuestGuild(const StoryId inStoryId, const QuestId inQuestId);			// 길드 퀘스트 보상 메세지 (서버 -> 클라)
	void RewardQuestGuide(const StoryId inStoryId, const QuestId inQuestId);			// 가이드 퀘스트 보상 메세지 (서버 -> 클라)
	void RewardBoxListQuestRepeat(const QuestIndex inQuestIndex, uint8 inRefreshCount, uint32 inRepeatScrollId,
		const TArray<RewardBoxItemIdPair>& inRewardBoxItemIdList);						// 반복 퀘스트 보상아이템 목록 (서버 -> 클라)

	void GiveUpQuest(IN const StoryId inStoryId, IN const QuestKey& inQuestKey);		// 포기
	void GiveUpQuest(IN const StoryId inStoryId, OUT QuestId& outQuestId);				// 메인 스토리 포기 메세지 (서버 -> 클라)
	void GiveUpQuestSub(const StoryId inStoryId);										// 서브 스토리 포기 메세지 (서버 -> 클라)
	void GiveUpQuestRepeat(const StoryId inStoryId, const QuestKey& inQuestKey);		// 반복 스토리 포기 메세지 (서버 -> 클라)
	void GiveUpQuestBoard(IN const StoryId inStoryId);									// 메인 스토리 포기 메세지 (서버 -> 클라)
	void GiveUpQuestGuild(IN const StoryId inStoryId);									// 메인 스토리 포기 메세지 (서버 -> 클라)
	void ClearQuest();
	void ClearQuest(QuestType inQuestType);

	void RemoveRankQuest(const StoryId inStoryId);										// 랭크 퀘스트 삭제

	void OnTouchQuest(const IGsMessageParam* In_data);									// QuestUI에서 터치 이벤트 발생 시 처리(수락,보상팝업을 띄우거나, 자동진행을 하거나..등등의 이벤트처리)
	bool OpenPopupQuestAccept(FGsQuestInstance* inQuest);								// 수락창을 팝업
	void OpenPopupQuestAccept(TSharedPtr<FGsQuestData> inQuestData, SpawnId inSpwanId);
	void OpenPopupQuestAccept(const IGsMessageParam* In_data);
	void OpenPopupQuestRepeatAccept(const QuestIndex inQuestIndex);	
	void OpenPopupQuestGuideAccept(const IGsMessageParam* In_data);
	/** 가이드 퀘스트에서 바로가기 기능 */
	void OnQuestGuideAutoProgress(const IGsMessageParam* In_data);

	bool OpenPopupQuestReward(FGsQuestInstance* inQuest);				// 보상창을 팝업
	void OpenPopupQuestRepeatReward(FGsQuestInstance* inQuest);			// 보상창을 팝업
	void OpenPopupQuestGuideReward(const IGsMessageParam* In_data);
	void OpenPopupQuestReward(const IGsMessageParam* In_data);
	void OnTeleport(const IGsMessageParam* In_data = nullptr);			// 텔레포트 요청
	void OnTeleport(FGsQuestInstance* inQuest);							// 텔레포트 요청
	void OnNpcToTeleport(const IGsMessageParam* In_data = nullptr);		// 서브 퀘스트 습득 전 npc 워프
	void OnNpcToAutoProgress(const IGsMessageParam* In_data);			// 서브 퀘스트 습득 전 npc로 자동이동 요청
	void ReQuestNpcToAutoProgress();									// 서브 퀘스트 습득 전 npc로 자동이동
	void OnQuestRepeatRewardRefresh(const IGsMessageParam* In_data);	// 퀘스트 반복 보상 새로고침
	void OnQuestRepeatComplete();										// 반복 퀘스트 중 상태가 Complete가 되었을때
	bool OnAutoTeleport(FGsQuestInstance* inQuest);						// 반복 퀘스트에서 오토로 자동진행 할때 워프 요청

	void OnQuestGiveUp(const IGsMessageParam* In_data = nullptr);		// 스토리 포기
	void OnUserAutoMoveStop(const IGsMessageParam* In_data = nullptr);	// 자동진행 중인게 있으면 취소	
	void SetAutoMoveStop(const IGsMessageParam* In_data = nullptr);		// 자동진행 중인게 있으면 취소	
	void SetAutoMoveStart(const IGsMessageParam* In_data = nullptr);	// 자동진행 중인게 있으면 시작	
	void OnActiveAIOffQuestAuto(const FGsInputEventMsgBase& In_msg);	// 퀘스트 자동이동 off, ai 자동전투 on
	void OnLocalTargetChanged(const IGsMessageParam* In_data = nullptr);// 타겟지정 수동조작
	void OffUIActionHudAuto();											// 퀘스트 자동 off, ai 자동 off

	void Init();
	void SpawnInit();													// 스폰시 하는 초기화

	void OnLocalPlayerDie(const IGsMessageParam* In_data = nullptr);	// 로컬 플레이어 사망
	void OnActiveAI(const IGsMessageParam* In_data = nullptr);			// ai on, off

	void AddRequestList(const QuestKey inQuestKey);						// 서버에 요청(수락, 완료)할 퀘스트아이디 추가
	void RemoveRequestList(const QuestKey inQuestKey);					// 서버에 응답(수락, 완료)받고 퀘스트아이디 삭제
	bool IsInRequestList(const QuestKey inQuestKey);					// 서버에 요청중인가?

	void UpdateQuestGuide();											// 가이드 퀘스트 갱신
	void RemoveQuestGuide(const StoryId inStoryId);						// 가이드 퀘스트 Instance 삭제
	void InvalidGuideInstance(const IGsMessageParam* In_data = nullptr);

	void OnUseWarpItem(const IGsMessageParam* InParam);					// 플레이어가 워프관련 아이템을 사용했을때
	void OnRequestContentsWarp(const IGsMessageParam* InParam);			// 플레이어가 워프를 동반한 컨텐츠에 입장할때 (던전..등등)

	bool IsInProgressList(const QuestKey inQuestKey);					// 진행중인 퀘스트인가?
	bool IsPossibleQuestWarp(const QuestKey inQuestKey);				// 워프가 가능한 상태인가?

private:
	void SetWarpReady(bool inIsValue) { _isWarpReady = inIsValue; }							// 워프 대기 상태 셋팅
	void SetUsingWarpRandomScroll(bool inIsValue) { _isUseWarpRandomScroll = inIsValue; }	// 랜덤 이동 주문서 사용 여부 셋팅
	bool IsWarpReady() { return _isWarpReady; }												// 워프 대기 상태인가?
	bool IsUsingWarpRandomScroll() { return _isUseWarpRandomScroll; }						// 랜덤 이동 주문서 사용 후 워프를 기다리는 중인가?
public:
	void ClearUsingWarpScroll();														// 랜덤 이동 주문서 사용여부 삭제

public:
	const TArray<FGsQuestInstance*>& GetQuesInstancetList() const { return _questInstanceList; }	// 현재 진행 중인 모든 퀘스트 리스트를 반환한다.
	FGsQuestTargetManagement* GetQuestTargetManagement() const;										// 퀘스트 타겟 매니지먼트 접근
	FGsQuestActionManagement* GetQuestActionManagement() const;										// 퀘스트 액션 매니지먼트 접근
	bool GetAutoMoveRemainDistance(OUT float& outRemainDistance);									// 자동이동 중이면 남은거리를 로컬pc 발밑에 표시한다.
	int32 GetQuestCount(QuestType inQuestType);														// 타입으로 퀘스트count 얻기	
	int32 GetQuestGuideCount();																		// 가이드 퀘스트 count 얻기
	const QuestState GetQuestState(QuestKey inQuestKey);											// 퀘스트상태 얻기	
	bool GetQuestObjectiveType(QuestKey inQuestKey, OUT QuestObjectiveType& outObjectiveType);		// 퀘스트 오브젝티브타입 얻기	
	FGsQuestInstance* GetQuestInstance(StoryId inStoryId, QuestKey inQuestKeyd);
	bool IsQuestGuideShowHud(StoryId inStoryId);													// 등록된 허드 중 스토리아이디로 찾기
	bool IsHasInteractionQuest(int32 inNpcTblId, int64 inGameId);									// npcid를 가진 인터랙션타입이 존재하는가?
	void QuestInstancetListSort();																	// 퀘스트 목록 정렬 규칙 적용

private:
	TSharedRef<FGsCostPackage> PopValidReservedCostPackage();

private:
	const struct FGsSchemaQuestRepeatScrollRefreshInfoSet* 
									GetRepeatRefrehTable(int32 inRepeatScrollId);					// 스크롤 ID로 반복퀘스트 챕터 정보 찾기
	bool GetQuestRepeatPopupParam(IN const QuestIndex inQuestIndex,
		OUT FGsQuestRepeatPopUpParam& outPopUpParam,
		OUT RewardType& outMainRewardType,
		OUT RewardType& outSubRewardType);															// 반복퀘스트 랜덤 보상 param 추출
	
	void RefrehPopupQuestRepeat(const QuestIndex inQuestIndex);										// 반복퀘스트 새로고침 적용

	/** 가이드 퀘스트 진행 중일 때는, 해당 컨텐츠에 해당하는 윈도우를 띄워준다 */
	void OpenCorrespondingWindow(const FGsSchemaQuestGuideStory* InSchemaQuestStory);

	/** 가이드 퀘스트에 맞는 텍스트 티커 띄워주기 */
	void ShowDetailDescText(const FGsSchemaQuestGuideStory* InSchemaQuestStory);


	// 모든 수락, 보상 팝업은 이 함수들을 통해 열도록 함
public:
	bool OpenPopupAccept(QuestType InType, const IGsMessageParam* InParam, RewardType InMainType, RewardType InSubType = RewardType::MAX, bool bIsTest = false);
	bool OpenPopupReward(const FGsQuestPopUpParamBase* InParam, RewardType InMainType, RewardType InSubType = RewardType::MAX, bool bIsTest = false);
};
