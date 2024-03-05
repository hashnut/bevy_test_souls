// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Tab/GsUIWindowQuestTabBase.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GsUIWindowQuestGuide.generated.h"

class UGsUIWindowQuestGuideItem;
class UTreeView;
class UGsUIWindowQuestGuideEntry;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestGuide : public UGsUIWindowQuestTabBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDetailDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherNonActive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAccept;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGuide;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richLockText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _canvasPanelQuestDsc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _storyIdText;

	UPROPERTY()
	TArray<class UGsUIWindowQuestGuideItem*> _treeRootSlotList;

	UPROPERTY()
	TArray<class UGsUIWindowQuestGuideItem*> _treeItemPool;

protected:
	TWeakObjectPtr<UGsUIWindowQuestGuideEntry> _selectedEntry;

	int32 _AcceptingStoryId = 0;
	int32 _scrollSelectIndex = 0;
	bool _isHudHideBtnEvent = false;
	int8 _slotState;

	TArray<TPair<MessageContentQuest, FDelegateHandle>>	_listQuestDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void BindMessage();
	void UnbindMessage();

protected:
	// 포기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReward();
	// 수락
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAccept();
	// 바로가기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickGuide();

public:
	virtual void UpdateList() override;
	virtual void RefreshAll() override;
	virtual void RefreshQuestObjective() override;
	void RefreshSlotState(const IGsMessageParam* In_data);


protected:
	UGsUIWindowQuestGuideItem* CreateTreeItem();
	void ReleaseTreeItem(UGsUIWindowQuestGuideItem* inItem);
	void ReleaseAllTreeItem();
	void FreeTreeItem();

protected:
	void OnEntryWidgetGenerated(UUserWidget& userWidget);
	void OnEntryWidgetReleased(UUserWidget& userWidget);
	void OnClickedListViewItem(UObject* InListItem);
	void OnClickHudUIOnOffBtn(int32 inStoryId, int32 inIsOn);

	void SetData(StoryId inStoryId);
	void SetEntryData(UUserWidget* userWidget);

public:
	void OnClickTab();

private:
	void OnClickedListEntry(UObject* InListItem);
};
