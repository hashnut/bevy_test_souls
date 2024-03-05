// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketSearchHistoryEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIMarketSearchHistoryEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnSlot->OnClicked.AddDynamic(this, &UGsUIMarketSearchHistoryEntry::OnClickSlot);
	_btnRemove->OnClicked.AddDynamic(this, &UGsUIMarketSearchHistoryEntry::OnClickRemove);
}

void UGsUIMarketSearchHistoryEntry::SetData(int32 InIndex, const FString& InData)
{
	_index = InIndex;
	_textBlockName->SetText(FText::FromString(InData));
}

void UGsUIMarketSearchHistoryEntry::OnClickSlot()
{
	// 선택하기 위해 슬롯 선택시에도 보내야 함
	OnClickEvent.ExecuteIfBound(_index, false);
}

void UGsUIMarketSearchHistoryEntry::OnClickRemove()
{
	OnClickEvent.ExecuteIfBound(_index, true);
}
