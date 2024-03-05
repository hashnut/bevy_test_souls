// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionCheckButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "Option/GsOptionEnumType.h"
#include "T1Project.h"

void UGsUIOptionCheckButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_switcherButton->OnClicked.AddDynamic(this, &UGsUIOptionCheckButton::OnClickButton);
}

bool UGsUIOptionCheckButton::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIOptionCheckButton::SetIsSelected(bool bInIsSelected)
{
	if(_bIsSelected == bInIsSelected)
		return;

	_switcherButton->SetIsSelected(_bIsSelected = bInIsSelected);

	if (_onClickButton.IsBound())
	{
		_onClickButton.ExecuteIfBound(_itemType, bInIsSelected);
	}
}

void UGsUIOptionCheckButton::OnClickButton()
{
	// 토글을 위해 부호 바꿈
	bool bIsChekced = !GetIsSelected();

	SetIsSelected(bIsChekced);
}

void UGsUIOptionCheckButton::SetData(const FText& InNameTag, bool InIsSelect, bool InIsEnabled, bool InisVisible)
{
	_textBlockOn->SetText(InNameTag);
	_textBlockOff->SetText(InNameTag);

	_switcherButton->SetIsEnabled(InIsEnabled ? true : false);
	SetIsSelected(InIsSelect);
}

void UGsUIOptionCheckButton::SetData(EGsOptionItem InType, const FText& InNameTag, bool InIsSelect, bool InIsEnabled, bool InisVisible)
{
	_itemType = InType;
	
	_textBlockOn->SetText(InNameTag);
	_textBlockOff->SetText(InNameTag);

	_switcherButton->SetIsEnabled(InIsEnabled ? true : false);
	_switcherButton->SetIsSelected(_bIsSelected = InIsSelect);
}