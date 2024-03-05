// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GsQuestInstance.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Message/GsMessageContents.h"

class FGsQuestMainTableCacheSet;
struct FGsSchemaQuestMainStory;
struct FGsQuestStoryTableCache;

/**
 *	퀘스트메인에 속한 진행 중인 퀘스트 인스턴스
 */
class T1PROJECT_API FGsQuestMainInstance final : public FGsQuestInstance, public IGsEventProgressInterface
{	
	using Super = FGsQuestInstance;

private:
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _messageContentQuestList;

private:		
	bool								_isStoryStart			= false;	// 스토리가 시작되었는가?
	bool								_isStoryRewarded		= false;	// 스토리가 완료되었는가?
	FGsQuestMainTableCacheSet*			_questMainTableCache	= nullptr;	// 퀘스트메인 테이블
	TSharedPtr<FGsQuestStoryTableCache>	_questStoryTableCache	= nullptr;	// 퀘스트서브 테이블
	const FGsSchemaQuestMainStory*		_schemaQuestStory;					// 스토리 스키마

public:
	virtual void Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey) override;
	// 퀘스트 상태 셋팅 후 처리
	virtual void SetQuestState(QuestState inState) override;
	// 초기화
	virtual void Clear();

	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void StartQuestEvent(QuestEventType inQuestEventType, int8 inindex = 0) override;

	virtual void OnPostGotoField() override;
protected:
	// 자동 이동
	virtual bool StartAutoProgress(bool inIsAuto = false) override;
	// 퀘스트 시작 연출
	virtual void PlayAcceptEffect() override;
	virtual bool IsShowHudUI() const;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	bool IsInitEventPlaying();
	void StartInitEvent(QuestEventType inQuestEventType);							// 첫 스토리 수락 전 하는 연출
	void StartStoryClearEvent(QuestEventType inQuestEventType);					// 스토리 끝났을때 하는 연출
	void StartStoryBeginEvent(QuestEventType inQuestEventType);					// 스토리가 시작됐을때 하는 연출
	//void PrAcceptBeginEvent(QuestEventType inQuestEventType);						// 재연결 시 끝내지 못한 연출 마저 연출
	
	//void StartObjectiveClearEvent(QuestEventType inQuestEventType, int8 inindex);	// 오브젝티브 수행 완료 됐을 때 하는 연출
	//void StartQuestClearEvent(QuestEventType inQuestEventType);
	//void StartQuestBeginEvent(QuestEventType inQuestEventType);
	
	void OnFinishedQuestEvent(const struct IGsMessageParam* inData);
	void SetQuestDynamicEventUpdate(QuestId inQuestId, bool isPreEventAction);	// 이벤트 액션 수행여부 업데이트

private:
	void AcceptQuest();								// 퀘스트 수락했을 때

	bool IsStoryRequireLevel();						// 스토리를 수행할수있는 레벨인가?

	void UpdateUIState(QuestState inState);			// 퀘스트 ui slot 갱신

public:
	// 퀘스트 습득레벨
	virtual const int32 GetRequireLevel() override;
	virtual void UpdateUIState() override;

public:
	// 퀘스트 습득레벨
	virtual void GetTitleText(OUT FText& outTitleText) override;
	virtual void GetDescText(OUT FText& outDescText) override;
	virtual const RewardId GetMainRewardId() const  override;
	virtual const RewardId GetSubRewardId() const  override;
	// 텔레포트 비용
	virtual bool GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const override;
};
