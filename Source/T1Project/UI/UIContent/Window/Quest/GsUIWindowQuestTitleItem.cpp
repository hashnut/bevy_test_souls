// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowQuestTitleItem.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

void UGsUIWindowQuestTitleItem::AddChild(UGsUIWindowQuestTitleItem * InItem)
{
	_childList.Add(InItem);
}

void UGsUIWindowQuestTitleItem::removeChild(UGsUIWindowQuestTitleItem* InItem)
{
	_childList.Remove(InItem);
}

void UGsUIWindowQuestTitleItem::removeAllChildren()
{
	for (UGsUIWindowQuestTitleItem* item : _childList)
	{
		_childList.Remove(item);
	}
	_childList.Reset();
}

void UGsUIWindowQuestTitleItem::OnGetChildren(UObject* InItem, TArray<UObject*>& InArray)
{
	UGsUIWindowQuestTitleItem* slot = Cast<UGsUIWindowQuestTitleItem>(InItem);
	if (slot)
	{
		TArray<UGsUIWindowQuestTitleItem*> childList = slot->GetChildList();

		if (0 < childList.Num())
		{
			InArray.Append(childList);
		}
	}
}