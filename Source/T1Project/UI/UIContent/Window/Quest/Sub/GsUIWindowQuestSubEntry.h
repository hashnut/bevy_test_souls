// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsSchemaEnums.h"
#include "SharedInclude/SharedTypes.h"
#include "GsUIWindowQuestSubEntry.generated.h"


class UImage;
class UTextBlock;
class UGsUIRedDotBase;
class UGsUIWindowQuestSubItem;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
class UVerticalBox;
struct FGsQuestSubChapterTableCache;
struct FGsQuestSubStoryTableCache;
class UGsButton;
class UGsUIWindowSubNPCListItem;


class UGsUIWindowSubAreaEntry;
class UGsUIWindowSubNpcEntry;
class UGsUIWindowSubStoryEntry;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestSubEntry :
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSelectImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _numText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _buttonSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _npcFaceImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherMapStateIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherNpcStateIcon;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;

	/*UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;*/

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowSubAreaEntry* _areaEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowSubNpcEntry* _npcEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIWindowSubStoryEntry* _storyEntry;

protected:
	TWeakObjectPtr<class UGsUIWindowQuestSubItem> _item;
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>> _storyList;	
	UGsUIWindowSubNPCListItem* _selectItem = nullptr;

protected:
	int _slotIndex = 0;
	int32 _mapId = 0;
	int32 _npcId = 0;
	bool _bIsSelected = false;
	bool _isMap = true;
	QuestContentsType _contentsType = QuestContentsType::NORMAL;
	StoryId _selectStoryId;
	StoryId _storyId;
	QuestSubItemType _subItemType;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

private:
	void OnStoryClickFunc(UGsUIWindowSubNPCListItem* inItem);
	void UpdateChildList(bool isVisible);

public:
	TWeakObjectPtr<class UGsUIWindowQuestSubItem> GetItem() { return _item; }

	void SetIsSelected(bool bInIsSelected);
	void SetSelect(bool inIsSelect);
	void SetSelectParents(bool inIsSelect);

	void Refresh();

	int32 GetMapId() { return _mapId; }
	int GetSlotIndex() { return _slotIndex; }
	int32 GetNpcId() { return _npcId; }

	StoryId GetStoryId();

	void SetDataMap(int inSlotIndex, int32 inMapId);
	void SetDataNpc(int32 inMapId, int32 inNpcId);
	void SetDataStory(int32 inMapId, int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inSubStoryTableCache);
};