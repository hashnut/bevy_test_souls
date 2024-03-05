// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSampleTreeItem.h"
#include "Components/TreeView.h"


void UGsSampleTreeItem::AddChild(UGsSampleTreeItem* InItem)
{
	_childList.Add(InItem);
}

void UGsSampleTreeItem::OnGetChildren(UObject* InItem, TArray<UObject*>& InArray)
{
	UGsSampleTreeItem* item = Cast<UGsSampleTreeItem>(InItem);
	if (item)
	{
		TArray<UGsSampleTreeItem*> childList = item->GetChildList();

		if (0 < childList.Num())
		{
			InArray.Append(childList);
		}
	}
}