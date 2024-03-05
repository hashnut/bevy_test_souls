// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "NoExportTypes.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "GsUIWindowQuestSubItem.generated.h"

class UGsUIWindowQuestSubEntry;
struct FGsQuestSubStoryTableCache;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestSubItem : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, const int32, const int32);
	DECLARE_DELEGATE_OneParam(FOnClickStorySlot, class UGsUIWindowQuestSubEntry*);

protected:
	UPROPERTY()
	TArray<UGsUIWindowQuestSubItem*> _childList;

public:
	FOnClickSlot  _delegateClickHudUIOnOffBtn;
	FOnClickStorySlot _delegateClickStorySlot;
	
private:
	int _parentsSlotIndex = 0;
	int _slotIndex = 0;

	int32 _mapId = 0;
	int32 _npcId = 0;
	StoryId _storyId = 0;

	TSharedPtr<FGsQuestSubStoryTableCache> _subStoryTableCache;

	QuestSubItemType _itemType = QuestSubItemType::SUB_AREA;

	QuestContentsType _contentsType = QuestContentsType::NORMAL;
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>> _npcStoryList;

public:
	void AddChild(UGsUIWindowQuestSubItem* InItem);
	void removeChild(UGsUIWindowQuestSubItem* InItem);
	void removeAllChildren();

	TArray<UGsUIWindowQuestSubItem*> GetChildList() const { return _childList; }
	void OnGetChildren(UObject* InItem, TArray<UObject*>& InArray);

public:
	void SetMapItem(int inSlotIndex, int32 inMapId);
	void SetNpcItem(int inParentsSlotIndex, int inSlotIndex, int32 inMapId, int32 inNpcId, StoryId inStoryId,
		QuestContentsType inContentsType, TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& inStoryList);	
	void SetStoryItem(int inParentsSlotIndex, int inSlotIndex, int32 inMapId, int32 inNpcId,
		QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inSubStoryTableCache);

	int32 GetMapId() { return _mapId; }

	int GetParentsSlotIndex() { return _parentsSlotIndex; }
	int GetSlotIndex() { return _slotIndex; }

	int32 GetNpcId() {	return _npcId; }
	StoryId GetStoryId() { return _storyId; }
	QuestContentsType GetQuestContentsType() { return _contentsType; }
	QuestSubItemType GetQuestSubItemType() { return _itemType; }
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& GetNpcStoryList() { return _npcStoryList; }
	TSharedPtr<FGsQuestSubStoryTableCache> GetSubStoryTableCache() { return _subStoryTableCache; }
};
