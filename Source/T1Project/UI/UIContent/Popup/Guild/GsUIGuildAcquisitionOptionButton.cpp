// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAcquisitionOptionButton.h"

#include "Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIGuildAcquisitionOptionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnUnSelected)
	{
		_btnUnSelected->OnClicked.AddDynamic(this, &UGsUIGuildAcquisitionOptionButton::OnClickSelect);
	}

	//if (_btnSelected)
	//{
	//	_btnSelected->OnClicked.AddDynamic(this, &UGsUIGuildAcquisitionOptionButton::OnClickUnSelect);
	//}
}

void UGsUIGuildAcquisitionOptionButton::OnClickSelect()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUIGuildAcquisitionOptionButton::OnClickUnSelect()
{
	//if (_toggleCallback)
	//{
	//	_toggleCallback(_indexInGroup, false);
	//}
}

void UGsUIGuildAcquisitionOptionButton::SetText(const FText& InText)
{
	_textBlockOn->SetText(InText);
	_textBlockOff->SetText(InText);
}

void UGsUIGuildAcquisitionOptionButton::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_btnUnSelected->SetVisibility(bInIsSelected ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	_btnSelected->SetVisibility(bInIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

bool UGsUIGuildAcquisitionOptionButton::GetIsSelected() const
{
	return _bIsSelected;
}
