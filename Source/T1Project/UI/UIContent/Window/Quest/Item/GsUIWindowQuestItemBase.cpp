// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowQuestItemBase.h"

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

void UGsUIWindowQuestItemBase::AddChild(UGsUIWindowQuestItemBase* InItem)
{
	_childList.Add(InItem);
}

void UGsUIWindowQuestItemBase::removeChild(UGsUIWindowQuestItemBase* InItem)
{
	_childList.Remove(InItem);
}

void UGsUIWindowQuestItemBase::removeAllChildren()
{
	for (UGsUIWindowQuestItemBase* item : _childList)
	{
		_childList.Remove(item);
	}
	_childList.Reset();
}

void UGsUIWindowQuestItemBase::SetData(int inIndex, int inMenuIndex, QuestWindowUISubSlotType inSlotType, const FText inTitleText)
{
	_index = inIndex;
	_slotType = inSlotType;
	_titleText = inTitleText;
	_menuIndex = inMenuIndex;
	_isVisible = true;
}

void UGsUIWindowQuestItemBase::SetData(int inIndex, QuestWindowUISubSlotType inSlotType, bool inIsVisible,
	const FText inTitleText, const FSoftObjectPath inIconPath)
{
	_index = inIndex;
	_slotType = inSlotType;
	_titleText = inTitleText;
	_iconPath = inIconPath;
	_isVisible = inIsVisible;
}

void UGsUIWindowQuestItemBase::SetData(int inIndex, StoryId inStoryId, int32 inNpcId, int inStoryIndex,
	QuestWindowUISubSlotType inSlotType, QuestWindowUISlotType inSlotStateType,
	bool inIsVisible, bool inIsSelecting, const FText inTitleText)
{
	_index = inIndex;
	_storyId = inStoryId;
	_npcId = inNpcId;
	_menuIndex = inStoryIndex;
	_isVisible = inIsVisible;
	_slotType = inSlotType;
	_slotStateType = inSlotStateType;
	_titleText = inTitleText;	
	_isSelecting = inIsSelecting;
}