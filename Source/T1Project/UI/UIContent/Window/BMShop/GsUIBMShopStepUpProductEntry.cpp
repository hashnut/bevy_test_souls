// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopStepUpProductEntry.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "GsUIBMShopProductEntry.h"
#include "GsUIBMShopSmallBanner.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "GsUIBMShopProductDetail.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"



void UGsUIBMShopStepUpProductEntry::BeginDestroy()
{
	if (nullptr != _productListPanelSlotHelper)
	{
		_productListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBMShopStepUpProductEntry::ProductListRefresh);
		_productListPanelSlotHelper = nullptr;
	}
	if (nullptr != _smallBannerListPanelSlotHelper)
	{
		_smallBannerListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBMShopStepUpProductEntry::SmallBannerListRefresh);
		_smallBannerListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBMShopStepUpProductEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _productListPanelSlotHelper)
	{
		_productListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_productListPanelSlotHelper->Initialize(_productListEntryWidgetClass, _productRootPanel);
		_productListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBMShopStepUpProductEntry::ProductListRefresh);
	}
	if (nullptr == _smallBannerListPanelSlotHelper)
	{
		_smallBannerListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_smallBannerListPanelSlotHelper->Initialize(_smallBannerListEntryWidgetClass, _bannerRootPanel);
		_smallBannerListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBMShopStepUpProductEntry::SmallBannerListRefresh);
	}

}

void UGsUIBMShopStepUpProductEntry::OnShow()
{
}

void UGsUIBMShopStepUpProductEntry::OnHide()
{
}

void UGsUIBMShopStepUpProductEntry::SetProduct(TArray<FGsBMShopProductData*> InProductList)
{
	_bannerId = 0;
	_productIdList.Empty();

	if (InProductList.IsValidIndex(0) && nullptr != InProductList[0])
	{
		_bannerId = InProductList[0]->GetProductID();
	}

	for (FGsBMShopProductData* iter : InProductList)
	{
		if(nullptr == iter)
			continue;

		_productIdList.Emplace(iter->GetProductID());
	}

	_productListPanelSlotHelper->RefreshAll(_productIdList.Num());
	_smallBannerListPanelSlotHelper->RefreshAll(1);
}

void UGsUIBMShopStepUpProductEntry::InvalidateProduct()
{
	_productListPanelSlotHelper->RefreshAll(_productIdList.Num());
	_smallBannerListPanelSlotHelper->RefreshAll(1);
}

void UGsUIBMShopStepUpProductEntry::ProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProductEntry* entry = Cast<UGsUIBMShopProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _productIdList.IsValidIndex(InIndex))
		return;

	entry->SetProductID(_productIdList[InIndex]);

	const TArray<UPanelSlot*>& slots = _productRootPanel->GetSlots();
	if (UPanelSlot* slot = slots[InIndex])
	{
		if (UGridSlot* gridSlot = Cast<UGridSlot>(slot))
		{
			gridSlot->SetRow(0);
			gridSlot->SetColumn(InIndex);
		}
	}
}

void UGsUIBMShopStepUpProductEntry::SmallBannerListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopSmallBanner* entry = Cast<UGsUIBMShopSmallBanner>(InEntry);
	if (nullptr == entry)
		return;

	if (0 >= _bannerId)
		return;

	entry->SetProductID(_bannerId);

	/*
	if (false == entry->OnClickSmallBanner.IsBoundToObject(this))
	{
		entry->OnClickSmallBanner.AddUObject(this, &UGsUIBMShopSBProductEntry::OnClickSmallBanner);
	}
	*/
}