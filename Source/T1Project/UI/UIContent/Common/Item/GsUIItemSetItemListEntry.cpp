// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIItemSetItemListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"


void UGsUIItemSetItemListEntry::SetTextName(const FText& InText)
{
	_textBlockNameOn->SetText(InText);
	_textBlockNameOff->SetText(InText);
}

void UGsUIItemSetItemListEntry::SetOnOff(bool bIsOn)
{
	// 0: off, 1: on,
	_switcher->SetActiveWidgetIndex(bIsOn ? 1 : 0);
}
