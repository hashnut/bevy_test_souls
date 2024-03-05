// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Quest/Main/GsUIWindowQuestMainItem.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

void UGsUIWindowQuestMainItem::AddChild(UGsUIWindowQuestMainItem* InItem)
{
	_childList.Add(InItem);
}

void UGsUIWindowQuestMainItem::removeChild(UGsUIWindowQuestMainItem* InItem)
{
	_childList.Remove(InItem);
}

void UGsUIWindowQuestMainItem::removeAllChildren()
{
	for (UGsUIWindowQuestMainItem* item : _childList)
	{
		_childList.Remove(item);
	}
	_childList.Reset();
}

void UGsUIWindowQuestMainItem::OnGetChildren(UObject* InItem, TArray<UObject*>& InArray)
{
	UGsUIWindowQuestMainItem* slot = Cast<UGsUIWindowQuestMainItem>(InItem);
	if (slot)
	{
		TArray<UGsUIWindowQuestMainItem*> childList = slot->GetChildList();

		if (0 < childList.Num())
		{
			InArray.Append(childList);
		}
	}
}

void UGsUIWindowQuestMainItem::SetChapterItem(int32 inChapterIndex)
{
	_isChapter = true;
	_chapterIndex = inChapterIndex;
}

void UGsUIWindowQuestMainItem::SetStoryItem(int32 inChapterIndex, int32 inStoryIndex, StoryId inStoryId)
{
	_isChapter = false;
	_chapterIndex = inChapterIndex;
	_storyIndex = inStoryIndex;
	_storyId = inStoryId;
}
