// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopProductEntry.h"
#include "T1Project.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "GsUIBMShopProductDetail.h"
#include "Management/GsMessageHolder.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"


void UGsUIBMShopProductEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnProduct->OnClicked.AddDynamic(this, &UGsUIBMShopProductEntry::OnClickIcon);
}

void UGsUIBMShopProductEntry::NativeDestruct()
{
	SetIsTutorialInteraction(false);

	Super::NativeDestruct();
}

void UGsUIBMShopProductEntry::OnShow()
{
	_bmShopMgr = GBMShop();
	_productID = 0;
	_productTID = 0;
	_isRunningTimer = false;
	_isPopupShopProduct = false;

	if (false == _btnProduct->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_btnProduct->_onFinishedLongPressGauge.AddUObject(this, &UGsUIBMShopProductEntry::OnLongPressIcon);
	}
}

void UGsUIBMShopProductEntry::OnHide()
{
}

void UGsUIBMShopProductEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
	else if(_isRunningTimer)
	{
		CountDownRemainTime();
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBMShopProductEntry::SetProductID(const BMProductId InDataID)
{
	if (nullptr == _bmShopMgr)
	{
		_bmShopMgr = GBMShop();
	}

	FGsBMShopProductData* data = _bmShopMgr->FindProduct(InDataID);
	if (nullptr == data)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopProductEntry::SetProductID] - error InDataID : %d"), InDataID);
		return;
	}

	_productID = data->GetProductID();
	_productTID = data->GetItemTID();

	SetProductName(data);
	SetSaleRemainTime(data);
	SetSaleCurrencyInfo(data);
	SetStepInfo(data);
	SetHighLightEffect(data);
	_detailInfo->SetProductData(data,true);

	//GSLOG(Error, TEXT("[UGsUIBMShopProductEntry::SetProductID] - _sizebox->WidthOverride : %f"), _sizebox->WidthOverride);
	
}

void UGsUIBMShopProductEntry::InvalidateProduct()
{
	FGsBMShopProductData* data = _bmShopMgr->FindProduct(_productID);
	if (nullptr == data)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopProductEntry::InvalidateProduct] - error _productID : %d"), _productID);
		return;
	}

	SetProductName(data);
	SetSaleRemainTime(data);
	SetSaleCurrencyInfo(data);
	SetStepInfo(data);
	_detailInfo->SetProductData(data,true);
}

void UGsUIBMShopProductEntry::SetHighLightEffect(const FGsBMShopProductData* InData)
{
	_highLightImg->SetVisibility(EBMShopHighlightType::Highlight_On == InData->GetHighLightType() ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIBMShopProductEntry::SetProductName(const FGsBMShopProductData* InData)
{
	_productNameText->SetText(InData->GetProductDisplayName());
}

void UGsUIBMShopProductEntry::SetStepInfo(const FGsBMShopProductData* InData)
{
	if (InData->IsStepUpProduct())
	{
		_stepNumRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		int32 maxCount = _bmShopMgr->GetMaxStepByGruopID(InData->GetStepUpGroupID());
		FString temp = FString::FromInt(InData->GetStepUpLevel()) + " / " + FString::FromInt(maxCount);
		_stepNumText->SetText(FText::FromString(temp));
		FGsBMShopManager::StepUpState state = _bmShopMgr->GetStepUpProductState(_productID);
		if (FGsBMShopManager::StepUpState::State_IsLock == state)
		{
			const FGsBMShopProductData* beforeData = _bmShopMgr->GetBeforeStepUpProductData(_productID);
			if (nullptr != beforeData)
			{
				FText findText;
				if (true == FText::FindText(TEXT("BMShopText"), TEXT("stepUpBan"), findText))
				{
					_stepWarnRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					FText resultText = findText.Format(findText, InData->GetStepUpLevel() - 1, beforeData->GetStepUpBuyLimitCount());
					_stepWarn->SetText(resultText);
				}
			}
		}
		else
		{
			_stepWarnRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		_stepWarnRootPanel->SetVisibility(ESlateVisibility::Collapsed);
		_stepNumRootPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIBMShopProductEntry::SetSaleRemainTime(const FGsBMShopProductData* InData)
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

void UGsUIBMShopProductEntry::SetSaleCurrencyInfo(const FGsBMShopProductData* InData)
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

void UGsUIBMShopProductEntry::CountDownRemainTime()
{
	FString result;
	if (true == _bmShopMgr->GetDisplayRemainTime(_productID, result))
	{
		_remainTimeText->SetText(FText::FromString(result));
	}
}

void UGsUIBMShopProductEntry::OnLongPressIcon()
{
	// 튜토리얼에서 롱프레스 불가
	if (IsTutorialInteraction())
	{	
		return;
	}

	FGsItemDetailOpenMessageParamTID param(_productTID, true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
}

void UGsUIBMShopProductEntry::OnClickIcon()
{
	if (IsTutorialInteraction())
	{
		OnTutorialInteraction.ExecuteIfBound();
	}

	FGsBMShopManager::DisplayBuyPopup(_productID);
}

float UGsUIBMShopProductEntry::GetWidthSize()
{
	return _sizebox->WidthOverride;
}

void UGsUIBMShopProductEntry::SetIsTutorialInteraction(bool bIsTutorial)
{ 
	_bIsTutorialInteraction = bIsTutorial;

	if (_bIsTutorialInteraction)
	{
		_detailInfo->OnTutorialInteraction.BindUObject(this, &UGsUIBMShopProductEntry::OnTutorialClickDetailInfoSlot);
		_detailInfo->SetIsTutorial(true);
	}
	else
	{
		_detailInfo->OnTutorialInteraction.Unbind();
		_detailInfo->SetIsTutorial(false);
	}
}

void UGsUIBMShopProductEntry::OnTutorialClickDetailInfoSlot()
{
	OnClickIcon();
}
