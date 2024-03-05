// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildStorageComboBoxEntry.h"

#include "Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUIGuildStorageComboBoxEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildStorageComboBoxEntry::OnClickSlot);
}

void UGsUIGuildStorageComboBoxEntry::NativeDestruct()
{
	_toggleCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildStorageComboBoxEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_btnSlot->SetIsSelected(bInIsSelected);
}

bool UGsUIGuildStorageComboBoxEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIGuildStorageComboBoxEntry::SetNameText(const FText& InText)
{
	_textBlockNormal->SetText(InText);
	_textBlockSelect->SetText(InText);
}

void UGsUIGuildStorageComboBoxEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
