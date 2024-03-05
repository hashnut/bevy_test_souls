// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopLargeBanner.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "PaperSprite.h"
#include "Classes/Engine/Texture2D.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"



void UGsUIBMShopLargeBanner::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	_btnOnBuy->OnClicked.AddDynamic(this, &UGsUIBMShopLargeBanner::OnBtnBuy);
}

void UGsUIBMShopLargeBanner::OnShow()
{
	_productID = 0;
	_isPopupShopProduct = false;
}

void UGsUIBMShopLargeBanner::OnHide()
{
	_productID = 0;
	_isPopupShopProduct = false;
}

void UGsUIBMShopLargeBanner::SetProductID(const BMProductId InProductID)
{
	_productID = InProductID;
	FGsBMShopManager* shopMgr = GBMShop();
	FGsBMShopProductData* data = shopMgr->FindProduct(_productID);
	if (nullptr == data)
		return;

	FString timeText = data->GetLimitSaleTimeText();
	FText resultText = FText::FromString(timeText);

	_priceSlot->SetBMShopCostData(data);
	if (CurrencyType::NONE == data->GetRebateCurrencyType() || 0 >= data->GetRebateAmount())
	{
		_mileageSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_mileageSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_mileageSlot->SetRebateCurrencyData(data->GetRebateCurrencyType(), data->GetRebateAmount());
	}

	_timerSwitcher->SetActiveWidgetIndex(data->IsPopupShopProduct() ? 1 : 0);
	// 판매 종료 시간 출력
	if (data->IsPopupShopProduct())
	{
		_isPopupShopProduct = shopMgr->GetPopupShopRemainTime(_productID, _popupShopTimer);
	}
	else
	{
		_saleTimeText->SetText(resultText);
	}

	SetImage(data);

	_couponWidget->SetVisibility(data->IsCouponProduct() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	// Test
	//_saleTimeText->SetText(data->GetName());
	//_limitTimeText->SetText(data->GetName());
}

void UGsUIBMShopLargeBanner::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

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
			SetProductID(_productID);
		}
	}
}

void UGsUIBMShopLargeBanner::SetImage(FGsBMShopProductData* InData)
{
	if (UGsLocalizationManager* localMgr = GLocalization())
	{
		UTexture2D* texture2D = localMgr->GetTexture(InData->GetBannerImgPath());
		if (nullptr != texture2D)
		{
			_image->SetBrushFromTexture(texture2D);
			_image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			_image->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	/*
	FSoftObjectPath convertPath(InData->GetBannerImgPath());
	UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(convertPath);
	if (nullptr != spriteImg)
	{
		_image->SetBrushFromAtlasInterface(spriteImg);
		_image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_image->SetVisibility(ESlateVisibility::Hidden);
	}
	*/
}

float UGsUIBMShopLargeBanner::GetWidthSize()
{
	return _sizebox->WidthOverride;
}

void UGsUIBMShopLargeBanner::OnBtnBuy()
{
	if (0 >= _productID)
		return;

	FGsBMShopManager::DisplayBuyPopup(_productID);
}