// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketCategoryListEntry.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUIMarketCategoryListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_sBtnSlot->OnClicked.AddDynamic(this, &UGsUIMarketCategoryListEntry::OnClickSlot);
}

void UGsUIMarketCategoryListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	if (_bIsSelected)
	{
		_sBtnSlot->SetIsSelected(true);
	}
	else
	{
		_sBtnSlot->SetIsSelected(false);
	}
}

void UGsUIMarketCategoryListEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 스왑을 위해 현재 상태에서 바꿔서 전달
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIMarketCategoryListEntry::SetUIName(const FText& InText)
{
	_textBlockNameOn->SetText(InText);
	_textBlockNameOff->SetText(InText);
}

void UGsUIMarketCategoryListEntry::SetUISubName(const FText& InText)
{
	_panelSubRoot->SetVisibility(InText.IsEmpty() ? 
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	_textBlockSubName->SetText(InText);
}
