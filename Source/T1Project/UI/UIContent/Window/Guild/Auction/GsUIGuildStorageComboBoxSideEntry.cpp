// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildStorageComboBoxSideEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIGuildStorageComboBoxSideEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGuildStorageComboBoxSideEntry::OnClickSlot);
}

void UGsUIGuildStorageComboBoxSideEntry::NativeDestruct()
{
	_toggleCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildStorageComboBoxSideEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
}

bool UGsUIGuildStorageComboBoxSideEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIGuildStorageComboBoxSideEntry::SetNameText(const FText& InText)
{
	_textBlockName->SetText(InText);
}

void UGsUIGuildStorageComboBoxSideEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
