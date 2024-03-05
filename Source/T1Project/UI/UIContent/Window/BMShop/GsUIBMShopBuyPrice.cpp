#include "GsUIBMShopBuyPrice.h"

#include "Runtime/UMG/Public/Components/PanelWidget.h"

#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

void UGsUIBMShopBuyPrice::NativeConstruct()
{
	Super::NativeConstruct();

	_discountRootPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBMShopBuyPrice::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIBMShopBuyPrice::SetData(const CurrencyType InType, const Currency InTotalAmount)
{
	_discountRootPanel->SetVisibility(ESlateVisibility::Collapsed);
	_totalAmount->SetChangeDisplayType(false);
	_totalAmount->SetData(InType, InTotalAmount);

	if (nullptr != _ownedArea)
	{
		_ownedArea->SetVisibility(InType == CurrencyType::CASH ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIBMShopBuyPrice::SetData(const int64 InItemTid, const Currency InAmount)
{
	_discountRootPanel->SetVisibility(ESlateVisibility::Collapsed);
	_totalAmount->SetChangeDisplayType(false);
	_totalAmount->SetData(InItemTid, InAmount);
	if (nullptr != _ownedArea)
	{
		_ownedArea->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIBMShopBuyPrice::SetDiscount(const CurrencyType InType, const Currency InDisCountAmount)
{
	_discountRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_discountAmount->SetData(InType, InDisCountAmount);
}

void UGsUIBMShopBuyPrice::SetDiscount(const uint32 InItemTid, const Currency InDisCountAmount)
{
	_discountRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_discountAmount->SetData(InItemTid, InDisCountAmount);
}

void UGsUIBMShopBuyPrice::SetOwnedAmountData(const CurrencyType InType)
{
	_ownedAmount->SetDataLocalPlayer(InType);
	_ownedAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (nullptr != _ownedArea)
	{
		_ownedArea->SetVisibility(InType == CurrencyType::CASH ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIBMShopBuyPrice::SetOwnedAmountData(const uint32 InItemTid)
{
	_ownedAmount->SetDataLocalPlayer(InItemTid);
	_ownedAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIBMShopBuyPrice::UpdateCostState()
{
	_totalAmount->UpdateAmountTextColorLocalPlayer();
}

void UGsUIBMShopBuyPrice::UpdateDiscountCostState(Currency InAmount)
{
	_discountAmount->UpdateDiscountAmountTextColorLocalPlayer(InAmount);
}

Currency UGsUIBMShopBuyPrice::GetDiscountAmount() const
{
	return _discountAmount->GetCurrencyAmount();
}

void UGsUIBMShopBuyPrice::SetCouponPriceData(const CurrencyType InType, const Currency InOriAmount, const Currency InDiscountAmount)
{
	_totalAmount->SetDisCountOriData(InType, InOriAmount);
	SetDiscount(InType, InDiscountAmount);
}

void UGsUIBMShopBuyPrice::SetCouponPriceData(const uint32 InItemTid, const Currency InOriAmount, const Currency InDiscountAmount)
{
	_totalAmount->SetDisCountOriData(InItemTid, InOriAmount);
	SetDiscount(InItemTid, InDiscountAmount);
}
