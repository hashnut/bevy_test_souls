// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Quest/Tab/GsUIWindowQuestTabBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowQuestMain.generated.h"

struct FGsQuestChapterData;
struct FGsQuestStoryTableCache;
class UGsButton;
class USizeBox;
class UGsUIWindowQuestMainItem;
class UTreeView;
class UGsUIWindowQuestMainEntry;
struct FGsSchemaQuestMainStory;

/**
 * 메인 퀘스트 스토리 디테일
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestMain : public UGsUIWindowQuestTabBase
{
	GENERATED_BODY()
	
protected:
	int32 _selectChapterIndex = 0;	
	//StoryId _selectStoryId = INVALID_STORY_ID;
	int32 _selectStoryIndex = 0;

	bool _isSlotClicked = false;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _btnSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnTeleport;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAutoMove;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _questIdText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _storyIdText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _lockLevelText;

	UPROPERTY()
	TArray<class UGsUIWindowQuestMainItem*> _treeRootSlotList;

	UPROPERTY()
	TArray<class UGsUIWindowQuestMainItem*> _treeItemPool;

	// 목록 리스트
	TArray<TSharedPtr<FGsQuestChapterData>> _chapterList;
	TSharedPtr<FGsQuestStoryTableCache> _storyTableCache;

protected:
	// 텔레포트
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickTeleport();

	// 텔레포트
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAutoMove();

	// 보상받기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReward();

	UFUNCTION()
	void OnClickClose();

	void OnEntryWidgetGenerated(UUserWidget& userWidget);
	void OnEntryWidgetReleased(UUserWidget& userWidget);
	void OnClickedListViewItem(UObject* InListItem);
	void OnItemExpansionChanged(UObject* InListItem, bool isValue);

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual void UpdateList() override;
	virtual void RefreshAll() override;

public:
	virtual void RefreshQuestObjective() override;

private:
	TSharedPtr<FGsQuestStoryTableCache> GetStoryTableCacheInIndex(int32 inChapterIndex, StoryId inStoryId);
	void SetData();
	void SetReward(const FGsSchemaQuestMainStory* inSchemaQuestStory);
	void ClearData();

protected:
	UGsUIWindowQuestMainItem* CreateTreeItem();
	void ReleaseTreeItem(UGsUIWindowQuestMainItem* inItem);
	void ReleaseAllTreeItem();
	void FreeTreeItem();

	void AllItemNonExpansion();

public:
	void OnClickTab(bool isOnTab);

	bool IsSlotClicked() { return _isSlotClicked; }
	void SetSlotClicked(bool inIsValue) { _isSlotClicked = inIsValue; }

private:
	void OnClickedListEntry(UObject* InListItem);

	UGsUIWindowQuestMainItem* GetChapterItem(int32 inChapterIndex);
	UGsUIWindowQuestMainItem* GetStoryItem(StoryId inStoryId);

	StoryId GetInChapterFirstStoryId(int32 InChapterIndex);
	StoryId GetInChapterStoryId(int32 InChapterIndex);
};
