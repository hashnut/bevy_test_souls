// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICurrencyButton.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUICurrencyButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnMain->OnClicked.AddDynamic(this, &UGsUICurrencyButton::OnClickMainButton);
}

void UGsUICurrencyButton::NativeDestruct()
{
	OnTutorialInteraction.Unbind();

	Super::NativeDestruct();
}

void UGsUICurrencyButton::SetButtonName(const FText& InText)
{
	_textButtonName->SetText(InText);
}

void UGsUICurrencyButton::OnClickMainButton()
{
	OnClickCurrencyButton.ExecuteIfBound();

	if (IsTutorialInteraction())
	{
		OnTutorialInteraction.ExecuteIfBound();
	}
}

void UGsUICurrencyButton::SetOnClickLockTime(float InSec)
{
	_btnMain->SetOnClickLockSec(InSec);
}

void UGsUICurrencyButton::SetButtonEnable(bool bEnable)
{
	_btnMain->SetIsEnabled(bEnable);
}
