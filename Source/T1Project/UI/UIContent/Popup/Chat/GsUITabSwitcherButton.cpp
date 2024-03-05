// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITabSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "TextBlock.h"

void UGsUITabSwitcherButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUITabSwitcherButton::NativeConstruct()
{
	Super::NativeConstruct();

	_btnSelect->OnClicked.AddDynamic(this, &UGsUITabSwitcherButton::OnButtonClick);
}

void UGsUITabSwitcherButton::NativeDestruct()
{
	_btnSelect->OnClicked.RemoveDynamic(this, &UGsUITabSwitcherButton::OnButtonClick);

	Super::NativeDestruct();
}

void UGsUITabSwitcherButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	_btnSelect->_slotIndexSelected = _slotIndexSelected;
	_btnSelect->_slotIndexUnselected = _slotIndexUnselected;

	if (_textBase)
	{
		_textBase->SetText(_textTabName);
		_textBase->SetColorAndOpacity(_tabBaseColor);
		_textBase->SynchronizeProperties();
	}
	if (_textSelect)
	{
		_textSelect->SetText(_textTabName);
		_textSelect->SetColorAndOpacity(_tabSelectColor);
		_textSelect->SynchronizeProperties();
	}
	if (_textDisable)
	{
		_textDisable->SetText(_textTabName);
		_textDisable->SetColorAndOpacity(_tabDisableColor);
		_textDisable->SynchronizeProperties();
	}
}

void UGsUITabSwitcherButton::OnButtonClick()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_btnSelect->GetIsSelected());
	}
}

void UGsUITabSwitcherButton::SetIsSelected(bool bInIsSelected)
{
	_btnSelect->SetIsSelected(bInIsSelected);
}

bool UGsUITabSwitcherButton::GetIsSelected() const
{
	return _btnSelect->GetIsSelected();
}

void UGsUITabSwitcherButton::SetSwitcherIndex(int32 InIndex)
{
	_btnSelect->SetSwitcherIndex(InIndex);
}

int32 UGsUITabSwitcherButton::GetSwitcherIndex() const
{
	return _btnSelect->GetSwitcherIndex();
}

FLinearColor UGsUITabSwitcherButton::GetTabColor()
{
	if (GetIsEnabled())		  return _tabDisableColor;
	else if (GetIsSelected()) return _tabSelectColor;

	return _tabBaseColor;
}
