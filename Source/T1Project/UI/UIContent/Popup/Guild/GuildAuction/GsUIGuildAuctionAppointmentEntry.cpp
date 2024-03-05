// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuctionAppointmentEntry.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIGuildAuctionAppointmentEntry::SetIsSelected(bool bInIsSelected)
{
	_isOn = bInIsSelected;
	_imageSelected->SetVisibility(bInIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

bool UGsUIGuildAuctionAppointmentEntry::GetIsSelected() const
{
	return _imageSelected->GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}

void UGsUIGuildAuctionAppointmentEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnName)
	{
		_btnName->OnClicked.AddDynamic(this, &UGsUIGuildAuctionAppointmentEntry::OnClickName);
	}
}

void UGsUIGuildAuctionAppointmentEntry::OnClickName()
{
	_isOn = !_isOn;
	_imageSelected->SetVisibility(_isOn ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	OnClickUserName.ExecuteIfBound(_userDBId, _isOn);

	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, _isOn);
	}
}

void UGsUIGuildAuctionAppointmentEntry::SetData(UserDBId InUserDBId, const FText& InUserName)
{
	_userDBId = InUserDBId;

	_textBlockUserName->SetText(InUserName);

	_isOn = false;
	_imageSelected->SetVisibility(ESlateVisibility::Hidden);
	_toggleCallback = nullptr;
}