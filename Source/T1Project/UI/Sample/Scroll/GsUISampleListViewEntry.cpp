// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISampleListViewEntry.h"
#include "UI/Sample/Scroll/GsSampleListItem.h"


void UGsUISampleListViewEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsSampleListItem* item = Cast<UGsSampleListItem>(ListItemObject))
	{
		_testText = FText::AsNumber(item->GetItemId());
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}
