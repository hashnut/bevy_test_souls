// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopSmallBanner.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Classes/Engine/Texture2D.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "PaperSprite.h"


void UGsUIBMShopSmallBanner::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	_btnOnClick->OnClicked.AddDynamic(this, &UGsUIBMShopSmallBanner::OnBtnProduct);
}

void UGsUIBMShopSmallBanner::OnShow()
{
	OnClickSmallBanner.Clear();
	_productID = 0;
	_isPopupShopProduct = false;
}

void UGsUIBMShopSmallBanner::OnHide()
{
	OnClickSmallBanner.Clear();
	_productID = 0;
}

float UGsUIBMShopSmallBanner::GetWidthSize()
{
	return _sizebox->WidthOverride;
}

void UGsUIBMShopSmallBanner::SetProductID(const BMProductId InProductID)
{
	_productID = InProductID;
	FGsBMShopManager* shopMgr = GBMShop();
	FGsBMShopProductData* data = shopMgr->FindProduct(_productID);
	if (nullptr == data)
		return;

	FString timeText = data->GetLimitSaleTimeText();
	FText resultText = FText::FromString(timeText);

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
}

void UGsUIBMShopSmallBanner::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UGsUIBMShopSmallBanner::SetImage(FGsBMShopProductData* InData)
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

void UGsUIBMShopSmallBanner::OnBtnProduct()
{
	if (0 >= _productID)
		return;

	FGsBMShopManager* bmShopMgr = GBMShop();
	FGsBMShopProductData* productData =  bmShopMgr->FindProduct(_productID);
	if (nullptr == productData)
		return;

	BMProductId id = productData->IsExistParentProductID() ? productData->GetParentProductID() : _productID;
	if (id != _productID)
	{
		productData = bmShopMgr->FindProduct(id);
		if (nullptr == productData)
			return;
	}

	// 해당 배너가 스탭업 상품일경우, 최종적으로 오픈되어 있는 스탭업 상품이 출력 되도록 함
	if (productData->IsStepUpProduct())
	{
		BMProductId curProductId = bmShopMgr->FindCurOpenStepUpProductId(productData->GetProductID());
		if (OnClickSmallBanner.IsBound())
		{
			OnClickSmallBanner.Broadcast(0 >= curProductId ? _productID : curProductId);
		}

		FGsBMShopManager::DisplayBuyPopup(0 >= curProductId ? _productID : curProductId);
	}
	else
	{
		if (OnClickSmallBanner.IsBound())
		{
			OnClickSmallBanner.Broadcast(_productID);
		}
		FGsBMShopManager::DisplayBuyPopup(_productID);
	}
}