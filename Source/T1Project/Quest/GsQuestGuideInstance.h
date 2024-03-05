// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsQuestInstance.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaQuestGuideStory;
struct FGsQuestGuideStoryTableCache;
/**
 *
 */
class T1PROJECT_API FGsQuestGuideInstance final : public FGsQuestInstance
{
	using Super = FGsQuestInstance;

public:
	virtual void Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey) override;
	virtual void SetQuestState(QuestState inState) override;

private:
	TSharedPtr<FGsQuestGuideStoryTableCache>	_questGuideTableCache;
	const FGsSchemaQuestGuideStory*				_schemaQuestStory;				// 퀘스트 길드 스토리 스키마

protected:
	virtual void Clear() override;
	// 자동 이동
	virtual bool StartAutoProgress(bool inIsAuto = false) override;

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

public:
	void UpdateObjectiveScanType();
	virtual void UpdateUIState() override;
};
