// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISampleTreeViewEntry.h"
#include "UI/Sample/Scroll/GsSampleTreeItem.h"
#include "Components/TreeView.h"


void UGsUISampleTreeViewEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsSampleTreeItem* item = Cast<UGsSampleTreeItem>(ListItemObject))
	{
		_testText = FText::AsNumber(item->GetItemId());
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGsUISampleTreeViewEntry::OnClickEntry()
{
	// 확장 메시지 보냄
	UTreeView* treeView = Cast<UTreeView>(GetOwningListView());
	if (treeView)
	{
		if (UObject* item = GetListItem<UObject>())
		{
			treeView->SetItemExpansion(item, !IsListItemExpanded());
		}
	}
}
