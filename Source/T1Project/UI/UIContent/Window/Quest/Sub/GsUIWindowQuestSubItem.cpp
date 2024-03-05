// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Sub/GsUIWindowQuestSubItem.h"

void UGsUIWindowQuestSubItem::AddChild(UGsUIWindowQuestSubItem* InItem)
{
	_childList.Add(InItem);
}

void UGsUIWindowQuestSubItem::removeChild(UGsUIWindowQuestSubItem* InItem)
{
	_childList.Remove(InItem);
}

void UGsUIWindowQuestSubItem::removeAllChildren()
{
	for (UGsUIWindowQuestSubItem* item : _childList)
	{
		_childList.Remove(item);
	}
	_childList.Reset();
}

void UGsUIWindowQuestSubItem::OnGetChildren(UObject* InItem, TArray<UObject*>& InArray)
{
	UGsUIWindowQuestSubItem* slot = Cast<UGsUIWindowQuestSubItem>(InItem);
	if (slot)
	{
		TArray<UGsUIWindowQuestSubItem*> childList = slot->GetChildList();

		if (0 < childList.Num())
		{
			InArray.Append(childList);
		}
	}
}

void UGsUIWindowQuestSubItem::SetMapItem(int inSlotIndex, int32 inMapId)
{
	_itemType = QuestSubItemType::SUB_AREA;

	_slotIndex = inSlotIndex;
	_mapId = inMapId;
}

void UGsUIWindowQuestSubItem::SetNpcItem(int inParentsSlotIndex, int inSlotIndex, int32 inMapId, int32 inNpcId, StoryId inStoryId,
	QuestContentsType inContentsType, TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& inStoryList)
{
	_itemType = QuestSubItemType::SUB_NPC;

	_parentsSlotIndex = inParentsSlotIndex;
	_slotIndex = inSlotIndex;
	_mapId = inMapId;
	_npcId = inNpcId;
	_contentsType = inContentsType;
	_npcStoryList = inStoryList;
	_storyId = inStoryId;
}

void UGsUIWindowQuestSubItem::SetStoryItem(int inParentsSlotIndex, int inSlotIndex, int32 inMapId, int32 inNpcId, QuestContentsType inContentsType, TSharedPtr<FGsQuestSubStoryTableCache> inSubStoryTableCache)
{
	_itemType = QuestSubItemType::SUB_STORY;

	_parentsSlotIndex = inParentsSlotIndex;
	_mapId = inMapId;
	_npcId = inNpcId;
	_subStoryTableCache = inSubStoryTableCache;
	_contentsType = inContentsType;
	_slotIndex = inSlotIndex;
}