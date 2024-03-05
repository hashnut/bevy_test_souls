// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIComboBoxEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIComboBoxEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIComboBoxEntry::OnClickSlot);
}

void UGsUIComboBoxEntry::NativeDestruct()
{
	_toggleCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIComboBoxEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

bool UGsUIComboBoxEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIComboBoxEntry::SetNameText(const FText& InText)
{
	_textBlockName->SetText(InText);
}

void UGsUIComboBoxEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
