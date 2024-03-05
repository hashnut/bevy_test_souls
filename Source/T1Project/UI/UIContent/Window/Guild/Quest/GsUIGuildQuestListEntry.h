// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUIGuildQuestListEntry.generated.h"

class UImage;
class UTextBlock;
class UPaperSprite;
class UGsSwitcherButton;
class UGsUIIconItemReward;
class UGsUIProgressBar;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
struct FGsGuildQuestData;
class FGsQuestObjectiveTableCache;
struct FGsSchemaQuestGuildStory;

/**
 * 기사단 협동 퀘스트 리스트 아이템 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildQuestListEntry : public UUserWidget,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIProgressBar* _progressBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelectorReward;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockZealPoint;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnState;

protected:
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _iconHelper;

protected:
	FGsIntervalChecker _checkerState;
	StoryId _storyId = INVALID_STORY_ID;
	QuestId _questId = INVALID_QUEST_ID;

	TArray<FGsRewardIconDataWrapper> _dataList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const FGsGuildQuestData* InData);

	void SetUIProgressBar(int32 InCurrCount, int32 InGoalCount);
	void SetIconImageSprite(UPaperSprite* InSprite);

protected:
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnLongPressIcon(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnClickState();

protected:
	const FGsQuestObjectiveTableCache* GetObjectiveTable(StoryId InStroyId);
	const FGsSchemaQuestGuildStory* GetStoryTable(StoryId InStroyId) const;
};
