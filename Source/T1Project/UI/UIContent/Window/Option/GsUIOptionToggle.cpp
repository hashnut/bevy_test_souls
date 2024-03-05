// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionToggle.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "Option/GsOptionEnumType.h"

void UGsUIOptionToggle::NativeOnInitialized()
{
	_btnNormal->OnClicked.AddDynamic(this, &UGsUIOptionToggle::OnClickNormal);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUIOptionToggle::OnClickSelected);

	Super::NativeOnInitialized();
}

void UGsUIOptionToggle::SetData(EGsOptionItem InType, bool InSelected, bool InIsEnabled)
{
	_optionType = InType;	

	SetSelected(InSelected);

	_btnNormal->SetIsEnabled(InIsEnabled);
	_btnSelected->SetIsEnabled(InIsEnabled);
}

void UGsUIOptionToggle::SetButtonText(const FText& InOnText, const FText& InOffText)
{
	_textOn->SetText(InOnText);
	_textOff->SetText(InOffText);
}

void UGsUIOptionToggle::OnClickNormal()
{
	SetSelected(true);
	OnClickSlot.ExecuteIfBound(_optionType, true);
}

void UGsUIOptionToggle::OnClickSelected()
{
	SetSelected(false);
	OnClickSlot.ExecuteIfBound(_optionType, false);
}

void UGsUIOptionToggle::SetSelected(bool bInSelected, bool InIsDisable)
{
	_bIsSelected = bInSelected;

	_btnNormal->SetVisibility((bInSelected) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_btnSelected->SetVisibility((bInSelected) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UGsUIOptionToggle::GetIsSelected() const
{
	return _bIsSelected;
}
