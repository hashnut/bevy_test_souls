// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupYesNoCurrency.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIPopupYesNoCurrency::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnNo->OnClicked.AddDynamic(this, &UGsUIPopupYesNoCurrency::OnClickNo);
	_btnYes->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupYesNoCurrency::OnClickYes);
}

void UGsUIPopupYesNoCurrency::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupYesNoCurrency::OnInputOk()
{
	// 재화량 체크
	if (false == FGsCurrencyHelper::CheckCurrency(_btnYes->GetCurrencyType(), _btnYes->GetCurrencyAmount()))
	{
		return;
	}

	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}
}

void UGsUIPopupYesNoCurrency::OnInputCancel()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupYesNoCurrency::OnClickYes()
{
	OnInputOk();
}

void UGsUIPopupYesNoCurrency::OnClickNo()
{
	OnInputCancel();
}

void UGsUIPopupYesNoCurrency::SetData(const FText& InContent, const FText& InYesButton, const FText& InNoButton,
	TFunction<void(bool)> InCallback)
{
	_richTextContent->SetText(InContent);
	_textBlockNo->SetText(InNoButton);
	_btnYes->SetButtonName(InYesButton);

	_buttonCallback = InCallback;
}

void UGsUIPopupYesNoCurrency::SetData(const FText& InContent, const FText& InYesButton, TFunction<void(bool)> InCallback)
{
	FText noText;
	FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonNo"), noText); // TEXT: 취소

	SetData(InContent, InYesButton, noText, InCallback);
}

void UGsUIPopupYesNoCurrency::SetCurrencyData(CurrencyType InType, Currency InAmount)
{
	_btnYes->SetData(InType, InAmount);
	_btnYes->UpdateAmountTextColorLocalPlayer();
}
