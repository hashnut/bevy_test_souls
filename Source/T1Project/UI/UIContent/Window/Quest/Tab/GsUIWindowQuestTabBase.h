// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsUIWindowQuestTabBase.generated.h"

class UTreeView;
class UGsUIWindowQuestTitleItem;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestTabBase : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FGsOnCloseQuestTab);

protected:
	// 스토리 타이틀
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _titleText;

	// 스토리 설명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _descriptionText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _stepText;

	// 획득 마을
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _townText;

	// 동시 수행 가능 횟수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _maxActiveCountText;

	// 주 보상
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIRewardInfoBase* _rewardInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTreeView* _treeView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _questText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _questWidget;

	StoryId _selectStoryId = 0;		

public:
	FGsOnCloseQuestTab _onCloseQuestTab;

protected:
	void SetTreeViewExpansion(UObject* inSlot, bool inIsExpansion);
	void SetQuestObjective(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX);
	
protected:
	virtual void NativeConstruct() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 메뉴 클릭
	UFUNCTION()
	virtual void OnClickSlot(int32 inChapterIndex, int32 inStoryIndex) {}

	// 슬롯 업데이트
	virtual void UpdateList();
	virtual void SetRewardData(int32 InRewardId, int32 InSubRewardId);

public:
	virtual void RefreshAll();
	virtual void RefreshQuestObjective() {}
	virtual void OnPage() {}

	virtual void OnGiveUp();
	virtual void OnTeleport();
	virtual void OnAutoMove();
	virtual void OnReward();
	virtual void OnClose();

	virtual QuestWindowUISlotType GetSlotType(StoryId inStoryId) 
					{ return QuestWindowUISlotType::STORY_PROGRESS; }
};
