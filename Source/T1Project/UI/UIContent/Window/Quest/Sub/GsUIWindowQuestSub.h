// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/Quest/Tab/GsUIWindowQuestTabBase.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsUIWindowQuestSub.generated.h"

class UGsUIWindowQuestItemBase;
class FText;
class USizeBox;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
class UTreeView;
class UGsUIWindowQuestSubItem;
class UTextBlock;
class UGsUIWindowQuestSubEntry;

UCLASS()
class T1PROJECT_API UGsUIWindowQuestSub : public UGsUIWindowQuestTabBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _btnSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _textSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnGiveUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnTeleport;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAutoMove;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _lockText;

	UPROPERTY()
	TArray<class UGsUIWindowQuestSubItem*> _treeRootSlotList;

	UPROPERTY()
	TArray<class UGsUIWindowQuestSubItem*> _treeItemPool;

// 개발용 ///////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _questIdText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _storyIdText;
// 개발용 ///////////////////////////////////////////////////////////////////////

protected:
	static const int32 INVALID_SLOT_INDEX = -1;

	QuestWindowUISlotType _slotStateType;
	StoryId _selectStoryId = 0;
	QuestId _selectQuestId = 0;
	FGsSchemaQuestObjectiveAreaPos _npcPos;
	int32 _npcId = 0;

	int32 _npcInteractionRange;

	int32 _mapSlotIndex = INVALID_SLOT_INDEX;
	int32 _npcSlotIndex = INVALID_SLOT_INDEX;
	int32 _storyIndex = INVALID_SLOT_INDEX;
	int32 _mapId = 0;
	bool _isAutoCloseSlot = false;

	TWeakObjectPtr<UGsUIWindowQuestSubEntry> _selectedAreaEntry;
	TWeakObjectPtr<UGsUIWindowQuestSubEntry> _selectedNpcEntry;
	TWeakObjectPtr<UGsUIWindowQuestSubEntry> _selectedStoryEntry;

protected:
	// 텔레포트
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickTeleport();

	// 자동이동
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAutoMove();

	// 포기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickGiveUp();

	UFUNCTION()
	void OnClickClose();

	// 보상받기
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickReward();

	void OnEntryWidgetGenerated(UUserWidget& userWidget);
	void OnEntryWidgetReleased(UUserWidget& userWidget);
	void OnClickedListViewItem(UObject* InListItem);
	void OnItemSelectionChanged(UObject* InListItem);

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void UpdateList() override;
	virtual void RefreshAll() override;

public:
	virtual void RefreshQuestObjective() override;

protected:
	UGsUIWindowQuestSubItem* CreateTreeItem();
	void ReleaseTreeItem(UGsUIWindowQuestSubItem* inItem);
	void ReleaseAllTreeItem();
	void FreeTreeItem();

public:
	void OnClickTab(bool isOnTab);
	
private:
	void SetData();
	bool GetTownNameText(CreatureId inNpcId, OUT FText& outNameText);

	bool IsDungeonMap(IN int32 inDungeonGroupId);
	bool GetIsMyMapId(int32 mapId);

	void ClearData();
	void SetCurrentMapScrollOffset(int inSlotIndex);
	void SetSelectCurrentMapSlot(int inSlotIndex);
	int GetMyMapIndex();


	void OnClickedArea(UGsUIWindowQuestSubItem* inItem);
	void OnClickedNpc(UGsUIWindowQuestSubItem* inItem);
	void OnClickedStory(UGsUIWindowQuestSubItem* inItem);

	void CloseArea();
	void CloseAreaSlot();
	void SelectNpcSlot(int32 inNpcIndex);

	void RefreshChild(UGsUIWindowQuestSubItem* inItem);

	void ClearIndex();
	void SetMyMapIndex();
	void SetMapIndex(int32 inMapIndex);
	void SetNpcIndex(int32 inNpcIndex);
	void SetStoryIndex(int32 inStroyIndex);

	void UpdateChildList(TWeakObjectPtr<class UGsUIWindowQuestSubItem> inSubItem, bool isVisible, int32 inIndex = 0);

private:
	void OnClickedListEntry(UObject* InListItem);
};