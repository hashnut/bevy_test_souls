// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupYesNoCurrencyRefreshCount.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencyRefreshCountButton.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIPopupYesNoCurrencyRefreshCount::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnNo->OnClicked.AddDynamic(this, &UGsUIPopupYesNoCurrencyRefreshCount::OnClickNo);
	_btnYes->OnClickRefreshButton.BindUObject(this, &UGsUIPopupYesNoCurrencyRefreshCount::OnClickYes);
}

void UGsUIPopupYesNoCurrencyRefreshCount::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupYesNoCurrencyRefreshCount::OnInputOk()
{
	// 재화량 체크
	bool bIsOk = true;
	if (false == FGsCurrencyHelper::CheckCurrency(_currencyType, _amount))
	{
		// 구매 팝업 열기
		Currency lackAmount = _amount - FGsCurrencyHelper::GetCurrencyAmount(_currencyType);
		FGsCurrencyHelper::OpenLackCurrencyPopup(_currencyType, lackAmount);

		bIsOk = false;
	}

	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(bIsOk);
	}
}

void UGsUIPopupYesNoCurrencyRefreshCount::OnInputCancel()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}


void UGsUIPopupYesNoCurrencyRefreshCount::SetData(const FText& InContent, int32 InRemainCount, CurrencyType InType, Currency InAmount,
	TFunction<void(bool)> InCallback)
{
	_richTextContent->SetText(InContent);
	_btnYes->SetData(InRemainCount, InType, InAmount);
	_currencyType = InType;
	_amount = InAmount;

	_buttonCallback = InCallback;
}

void UGsUIPopupYesNoCurrencyRefreshCount::OnClickYes()
{
	OnInputOk();
}

void UGsUIPopupYesNoCurrencyRefreshCount::OnClickNo()
{
	OnInputCancel();
}
