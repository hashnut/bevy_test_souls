// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketSubCategoryListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIMarketSubCategoryListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnSlot->OnClicked.AddDynamic(this, &UGsUIMarketSubCategoryListEntry::OnClickSlot);
}

void UGsUIMarketSubCategoryListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	//_selectedUI->SetVisibility(_bIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMarketSubCategoryListEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 스왑을 위해 현재 상태에서 바꿔서 전달
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIMarketSubCategoryListEntry::SetUIName(const FText& InText)
{
	_textBlockName->SetText(InText);
}