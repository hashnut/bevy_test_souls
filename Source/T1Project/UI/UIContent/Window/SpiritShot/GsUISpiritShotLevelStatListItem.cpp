// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotLevelStatListItem.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

void UGsUISpiritShotLevelStatListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUISpiritShotLevelStatListItem::NativeOnListItemObjectSet(UObject* InListItemData)
{
	UGsUISpiritShotLevelStatListData* itemData = Cast<UGsUISpiritShotLevelStatListData>(InListItemData);
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}


	// 레벨 도달 여부에 따라 스탯 표시 색상 변경
	if (itemData->IsLevelReached())
	{
		_widgetSwitcherTextColor->SetActiveWidgetIndex(0);
		_txtLevelBefore->SetText(FText::FromString(FString::FromInt(itemData->GetLevel())));
		_txtStatNameBefore->SetText(itemData->GetStatName());
		_txtStatValueBefore->SetText(itemData->GetStatValue());
	}
	else
	{
		_widgetSwitcherTextColor->SetActiveWidgetIndex(1);
		_txtLevelAfter->SetText(FText::FromString(FString::FromInt(itemData->GetLevel())));
		_txtStatNameAfter->SetText(itemData->GetStatName());
		_txtStatValueAfter->SetText(itemData->GetStatValue());
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemData);
}
