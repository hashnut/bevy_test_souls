// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Guide/GsUIWindowQuestGuideItem.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"


void UGsUIWindowQuestGuideItem::AddChild(UGsUIWindowQuestGuideItem* InItem)
{
	_childList.Add(InItem);
}

void UGsUIWindowQuestGuideItem::removeChild(UGsUIWindowQuestGuideItem* InItem)
{
	_childList.Remove(InItem);
}

void UGsUIWindowQuestGuideItem::removeAllChildren()
{
	for (UGsUIWindowQuestGuideItem* item : _childList)
	{
		_childList.Remove(item);
	}
	_childList.Reset();
}

void UGsUIWindowQuestGuideItem::OnGetChildren(UObject* InItem, TArray<UObject*>& InArray)
{
	UGsUIWindowQuestGuideItem* slot = Cast<UGsUIWindowQuestGuideItem>(InItem);
	if (slot)
	{
		TArray<UGsUIWindowQuestGuideItem*> childList = slot->GetChildList();

		if (0 < childList.Num())
		{
			InArray.Append(childList);
		}
	}
}

void UGsUIWindowQuestGuideItem::SetStoryItem(int inSlotIndex, StoryId inStoryId)
{
	_slotIndex = inSlotIndex;
	_storyId = inStoryId;
}