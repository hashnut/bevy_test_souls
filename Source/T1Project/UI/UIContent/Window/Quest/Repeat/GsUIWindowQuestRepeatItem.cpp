// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Repeat/GsUIWindowQuestRepeatItem.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"


void UGsUIWindowQuestRepeatItem::AddChild(UGsUIWindowQuestRepeatItem* InItem)
{
	_childList.Add(InItem);
}

void UGsUIWindowQuestRepeatItem::removeChild(UGsUIWindowQuestRepeatItem* InItem)
{
	_childList.Remove(InItem);
}

void UGsUIWindowQuestRepeatItem::removeAllChildren()
{
	for (UGsUIWindowQuestRepeatItem* item : _childList)
	{
		_childList.Remove(item);
	}
	_childList.Reset();
}

void UGsUIWindowQuestRepeatItem::OnGetChildren(UObject* InItem, TArray<UObject*>& InArray)
{
	UGsUIWindowQuestRepeatItem* slot = Cast<UGsUIWindowQuestRepeatItem>(InItem);
	if (slot)
	{
		TArray<UGsUIWindowQuestRepeatItem*> childList = slot->GetChildList();

		if (0 < childList.Num())
		{
			InArray.Append(childList);
		}
	}
}

void UGsUIWindowQuestRepeatItem::SetQuestItem(StoryId inStoryId, QuestKey inQuestKey, int inSlotIndex)
{
	_storyId = inStoryId;
	_questKey = inQuestKey;
	_slotIndex = inSlotIndex;
	_isFirst = false;
	_isLast = false;
}

void UGsUIWindowQuestRepeatItem::SetQuestItem(StoryId inStoryId, QuestKey inQuestKey, int inSlotIndex, bool inIsFirst, bool inIsLast)
{
	_storyId = inStoryId;
	_questKey = inQuestKey;
	_slotIndex = inSlotIndex;
	_isFirst = inIsFirst;
	_isLast = inIsLast;
}

void UGsUIWindowQuestRepeatItem::ReSetQuestItem(StoryId inStoryId, QuestKey inQuestKey)
{
	_storyId = inStoryId;
	_questKey = inQuestKey;
	_isFirst = false;
	_isLast = false;
}