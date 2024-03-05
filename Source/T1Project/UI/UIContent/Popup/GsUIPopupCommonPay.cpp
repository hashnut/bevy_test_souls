// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommonPay.h"
#include "RichTextBlock.h"
#include "TextBlock.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Currency/GsCostPackage.h"
#include "T1Project.h"


void UGsUIPopupCommonPay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCommonPay::OnClickCancel);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCommonPay::OnClickOk);
}

void UGsUIPopupCommonPay::NativeConstruct()
{
	Super::NativeConstruct();

	_underContentRichText->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupCommonPay::NativeDestruct()
{
	_buttonCallback = nullptr;

	if (nullptr != _destructCallback)
	{
		_destructCallback(_isCallEventProcessFinish);
		_destructCallback = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIPopupCommonPay::OnInputOk()
{
	// 재화가 부족할 경우 cancel처럼 처리 후 재화부족 팝업 열기
	if (!_costPackage->PlayerHasEnough())
	{
		_isCallEventProcessFinish = true;

		// 즉시 닫기
		Close(true);

		if (nullptr != _buttonCallback)
		{
			_buttonCallback(false);
		}

		FGsCurrencyHelper::OpenLackCostPackagePopup(*_costPackage);
	}
	else
	{
		_isCallEventProcessFinish = false;

		Close();

		if (nullptr != _buttonCallback)
		{
			_buttonCallback(true);
		}
	}
}

void UGsUIPopupCommonPay::OnInputCancel()
{
	_isCallEventProcessFinish = true;

	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupCommonPay::SetData(const FText& InContent, TSharedRef<FGsCostPackage> InCostPackage,
	TFunction<void(bool)> InCallback /*= nullptr*/, TFunction<void(bool)> InDestructCallback /*= nullptr*/)
{
	_costPackage = MoveTemp(InCostPackage);

	_buttonCallback = InCallback;
	_contentRichText->SetText(InContent);
	_destructCallback = InDestructCallback;
	_isCallEventProcessFinish = true;

	// 필요 재화
	const FGsCostElement* CostElement = _costPackage->GetFirstCostElement();
	if (nullptr == CostElement)
	{
		// 비용 요소가 하나도 없는데, 비용 처리 팝업을 오픈한 경우로, 이 블럭을 실행하는 것은 에러로 간주해야 함.
		GSLOG(Error, TEXT("nullptr == CostElement. Need a costelement. Forcely add zero gold."));
		return;
	}

	_currencyCommonPay->SetData(*CostElement);
	_currencyCommonPay->SetAmountTextColor(CostElement->PlayerHasEnough() ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);

	// 보유 재화
	if (CostType::CURRENCY == CostElement->_costType)
	{
		_currencyLocalPay->SetData(CostElement->GetCurrencyType(), CostElement->PlayerHasAmountMax());
	}
	else if (CostType::ITEM == CostElement->_costType)
	{
		_currencyLocalPay->SetData(CostElement->GetItemId(), CostElement->PlayerHasAmountMax());
	}
	else
	{
		GSLOG(Error, TEXT("Invalid CostElement->_costType"));
	}
}

void UGsUIPopupCommonPay::SetUnderText(const FText& InText)
{
	if (InText.IsEmpty())
	{
		_underContentRichText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_underContentRichText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_underContentRichText->SetText(InText);
}

void UGsUIPopupCommonPay::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupCommonPay::OnClickCancel()
{
	OnInputCancel();
}
