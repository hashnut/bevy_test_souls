// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotPlusStatListItem.h"

#include "UMG/Public/Components/TextBlock.h"
#include "T1Project.h"


void UGsUISpiritShotPlusStatListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUISpiritShotPlusStatListItem::NativeOnListItemObjectSet(UObject* InListItemData)
{
	UGsSpiritShotPlusStatListData* itemData = Cast<UGsSpiritShotPlusStatListData>(InListItemData);
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	_txtStatName->SetText(itemData->GetStatName());
	_txtStatValue->SetText(itemData->GetStatValue());

	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemData);
}