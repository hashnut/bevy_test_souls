// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRadioButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"


void UGsUIRadioButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_switcherButton->OnClicked.AddDynamic(this, &UGsUIRadioButton::OnClickButton);
}


void UGsUIRadioButton::NativeConstruct()
{
	Super::NativeConstruct();

	_switcherButton->SetSwitcherIndex(0);
}

void UGsUIRadioButton::SetIsSelected(bool bInIsSelected)
{
	_switcherButton->SetIsSelected(bInIsSelected);

	if (OnRadioButtonSelected.IsBound())
	{
		OnRadioButtonSelected.Broadcast(bInIsSelected);
	}
}

bool UGsUIRadioButton::GetIsSelected() const
{
	return _switcherButton->GetIsSelected();
}

IGsToggleGroupEntry* UGsUIRadioButton::GetToggleGroupEntry()
{
	return _switcherButton;
}

void UGsUIRadioButton::OnClickButton()
{
	SetIsSelected(true);
}