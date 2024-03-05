// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopDetailProductEntry.h"
#include "T1Project.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "GsUIBMShopProductDetail.h"
#include "Components/WidgetSwitcher.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"


void UGsUIBMShopDetailProductEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnProduct->OnClicked.AddDynamic(this, &UGsUIBMShopDetailProductEntry::OnClickIcon);
}

void UGsUIBMShopDetailProductEntry::OnShow()
{
	_bmShopMgr = GBMShop();
	_productID = 0;
	_productTID = 0;
	_isRunningTimer = false;
	_isPopupShopProduct = false;

	if (false == _btnProduct->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_btnProduct->_onFinishedLongPressGauge.AddUObject(this, &UGsUIBMShopDetailProductEntry::OnLongPressIcon);
	}
}

void UGsUIBMShopDetailProductEntry::OnHide()
{
}

void UGsUIBMShopDetailProductEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_isPopupShopProduct)
	{
		FTimespan span = _popupShopTimer - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
		if (0 <= span.GetTicks())
		{
			FText findText;
			FString timeStr;
			FGsTimeStringHelper::GetTimeStringHMS(span, timeStr, false);
			FText::FindText(TEXT("BMShopText"), TEXT("leftSaleTime"), findText);
			FText text = FText::Format(findText, FText::FromString(timeStr));
			_limitTimeText->SetText(text);
		}
		else
		{
			InvalidateProduct();
		}
	}
	else if (_isRunningTimer)
	{
		CountDownRemainTime();
	}


	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBMShopDetailProductEntry::SetProductID(const BMProductId InDataID)
{
	if (nullptr == _bmShopMgr)
	{
		_bmShopMgr = GBMShop();
	}

	FGsBMShopProductData* data = _bmShopMgr->FindProduct(InDataID);
	if (nullptr == data)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopDetailProductEntry::SetProductID] - error InDataID : %d"), InDataID);
		return;
	}

	_productID = data->GetProductID();
	_productTID = data->GetItemTID();

	SetProductName(data);
	SetSaleRemainTime(data);
	SetSaleCurrencyInfo(data);
	SetExternalDesc(data);
	SetHighLightEffect(data);
	_detailInfo->SetProductData(data);
}

void UGsUIBMShopDetailProductEntry::InvalidateProduct()
{
	FGsBMShopProductData* data = _bmShopMgr->FindProduct(_productID);
	if (nullptr == data)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopDetailProductEntry::InvalidateProduct] - error _productID : %d"), _productID);
		return;
	}

	SetProductName(data);
	SetSaleRemainTime(data);
	SetSaleCurrencyInfo(data);
	_detailInfo->SetProductData(data);
}

void UGsUIBMShopDetailProductEntry::SetHighLightEffect(const FGsBMShopProductData* InData)
{
	_highLightImg->SetVisibility(EBMShopHighlightType::Highlight_On == InData->GetHighLightType() ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIBMShopDetailProductEntry::SetExternalDesc(const FGsBMShopProductData* InData)
{
	_externalDescRichText->SetText(InData->GetExternalDesc());
}

void UGsUIBMShopDetailProductEntry::SetProductName(const FGsBMShopProductData* InData)
{
	_productNameText->SetText(InData->GetProductDisplayName());
}

void UGsUIBMShopDetailProductEntry::SetSaleRemainTime(const FGsBMShopProductData* InData)
{
	_isPopupShopProduct = _bmShopMgr->GetPopupShopRemainTime(_productID, _popupShopTimer);
	_timerSwitcher->SetActiveWidgetIndex(_isPopupShopProduct ? 1 : 0);
	if (!_isPopupShopProduct)
	{
		FString remainTime = InData->GetDisplayTime();
		if (true == remainTime.IsEmpty())
		{
			_isRunningTimer = false;
			_remainTimeText->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}

		_remainTimeText->SetVisibility(ESlateVisibility::Visible);
		//_remainTimeText->SetText(FText::FromString(remainTime));

		FDateTime time;
		// 판매 시작전은 무조건 표기함
		if (true == InData->IsBeforSale())
		{
			InData->GetSaleStartTime(time);
		}
		else // 판매 시작 이후, 시간 표기는 countDownTime 에 의하여 표기함
		{
			InData->GetSaleEndTime(time);
		}

		_bmShopMgr->SetProductRemainTime(_productID, time);
		_isRunningTimer = true;
	}
}

void UGsUIBMShopDetailProductEntry::SetSaleCurrencyInfo(const FGsBMShopProductData* InData)
{
	_currencySlot->SetBMShopCostData(InData);

	if (CurrencyType::NONE == InData->GetRebateCurrencyType())
	{
		_mileageCurrencySlot->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_mileageCurrencySlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_mileageCurrencySlot->SetRebateCurrencyData(InData->GetRebateCurrencyType(), InData->GetRebateAmount());
}

void UGsUIBMShopDetailProductEntry::CountDownRemainTime()
{
	FString result;
	if (true == _bmShopMgr->GetDisplayRemainTime(_productID, result))
	{
		_remainTimeText->SetText(FText::FromString(result));
	}

	/*
	*	GSLOG(Warning, TEXT("[CountDownRemainTime] - _targetTime.GetDays() : %d"), _targetTime->GetDay());
			GSLOG(Warning, TEXT("[CountDownRemainTime] - _targetTime.GetHours() : %d"), _targetTime->GetHour());
			GSLOG(Warning, TEXT("[CountDownRemainTime] - _targetTime.GetMinutes() : %d"), _targetTime->GetMinute());
	*/
}

void UGsUIBMShopDetailProductEntry::OnLongPressIcon()
{
	FGsItemDetailOpenMessageParamTID param(_productTID, true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
}

void UGsUIBMShopDetailProductEntry::OnClickIcon()
{
	FGsBMShopManager::DisplayBuyPopup(_productID);
}

float UGsUIBMShopDetailProductEntry::GetWidthSize()
{
	return _sizebox->WidthOverride;
}