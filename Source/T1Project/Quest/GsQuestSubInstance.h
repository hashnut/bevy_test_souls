// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsQuestInstance.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsQuestSubTableCacheSet;
struct FGsSchemaQuestSubStory;
/**
 * 
 */
class T1PROJECT_API FGsQuestSubInstance final : public FGsQuestInstance
{
	using Super = FGsQuestInstance;

private:
	TSharedPtr<FGsQuestSubStoryTableCache>	_questSubTableCache = nullptr;	// 퀘스트서브 테이블
	const FGsSchemaQuestSubStory*			_schemaQuestStory;				// 퀘스트 서브 스토리 스키마
	int _npcTblId;

public:
	virtual void Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey) override;
	virtual void SetQuestState(QuestState inState) override;

protected:
	virtual void Clear() override;
	// 다음 스토리에 연퀘 시작
	virtual void AddNextStoryQuest() override;
	// 다음 연퀘 시작
	virtual void AddNextQuest() override;
	// 퀘스트 시작 연출
	virtual void PlayAcceptEffect() override;
	// 자동 이동
	virtual bool StartAutoProgress(bool inIsAuto = false) override;

	virtual void StartQuestEvent(QuestEventType inQuestEventType, int8 inindex = 0) override;
	virtual void StartQuestClearEvent(QuestEventType inQuestEventType) override;

private:
	void OpenNpcDialog(const int64 inGameId);

public:	
	virtual const int32 GetRequireLevel() override;					// 퀘스트 습득레벨
	virtual bool IsShowHudUI() const;								// hudUI에 표시여부?

public:
	// 퀘스트 UI 스트링
	virtual void GetTitleText(OUT FText& outTitleText) override;
	virtual void GetDescText(OUT FText& outDescText) override;
	virtual const RewardId GetMainRewardId() const  override;
	virtual const RewardId GetSubRewardId() const  override;
	// 텔레포트 비용
	virtual bool GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const override;
};
