// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopProductItem.h"
#include "Item/GsItem.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Currency/GsCurrencyHelper.h"
#include "Reward/GsRewardHelper.h"



void UGsUIBMShopProductItem::BeginDestroy()
{
	if (nullptr != _itemiconSelector)
	{
		_itemiconSelector = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBMShopProductItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDetail->OnClicked.AddDynamic(this, &UGsUIBMShopProductItem::OnDetail);

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_itemiconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBMShopProductItem::OnRefreshItemIcon);
	}
}

void UGsUIBMShopProductItem::OnShow()
{
	_itemBagData = nullptr;
	_btnDetail->SetVisibility(ESlateVisibility::Collapsed);

	_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
	_cashRebateInfo.mCurrency = 0;
}

void UGsUIBMShopProductItem::OnHide()
{
	_itemBagData = nullptr;
	_iconSlotHelper->Clear();

	_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
	_cashRebateInfo.mCurrency = 0;
}

void UGsUIBMShopProductItem::SetData(const FGsSchemaRewardItemBagData* InBagData)
{
	if (nullptr == InBagData)
		return;

	_itemBagData = InBagData;
	bool isItemType = 0 < _itemBagData->itemId;
	if (isItemType)
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(_itemBagData->itemId);
		if (nullptr != itemData)
		{
			_itemNameText->SetText(itemData->name);
			_btnDetail->SetVisibility(ItemCategorySub::REWARD_BOX == itemData->categorySub ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
	else
	{
		FText name = FGsCurrencyHelper::GetCurrencyLocalizedText(_itemBagData->currencyType);
		_itemNameText->SetText(name);
		_btnDetail->SetVisibility(ESlateVisibility::Collapsed);
	}

	_iconSlotHelper->RefreshAll(1);
}

void UGsUIBMShopProductItem::SetData(CurrencyPair InCurrencyPairData)
{
	_itemBagData = nullptr;

	FText name = FGsCurrencyHelper::GetCurrencyLocalizedText(InCurrencyPairData.mCurrencyType);
	_itemNameText->SetText(name);
	_btnDetail->SetVisibility(ESlateVisibility::Collapsed);

	_cashRebateInfo = InCurrencyPairData;

	_iconSlotHelper->RefreshAll(1);
}

void UGsUIBMShopProductItem::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (CurrencyType::NONE != _cashRebateInfo.mCurrencyType)
	{
		itemIcon->SetCurrency(_cashRebateInfo.mCurrencyType, 1);
		itemIcon->SetItemCountMinMax(_cashRebateInfo.mCurrency, _cashRebateInfo.mCurrency);
	}
	else
	{
		if (nullptr == _itemBagData || (0 >= _itemBagData->itemId && CurrencyType::NONE == _itemBagData->currencyType))
		{
			itemIcon->SetNotOwnItem(0, 0);
			itemIcon->SetItemNameToolTipFlag(false);
			itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
			return;
		}

		bool isItemType = 0 < _itemBagData->itemId;
		if (isItemType)
		{
			itemIcon->SetNotOwnItem(_itemBagData->itemId, 1, _itemBagData->itemLevel);
			itemIcon->SetItemCountMinMax(_itemBagData->itemCountMin, _itemBagData->itemCountMax);
		}
		else
		{
			itemIcon->SetCurrency(_itemBagData->currencyType, 1);
			itemIcon->SetItemCountMinMax(_itemBagData->currencyAmountMin, _itemBagData->currencyAmountMax);
		}
	}

	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetRedDot(false);

	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIBMShopProductItem::OnLongPressIcon);
	}
}

void UGsUIBMShopProductItem::OnDetail()
{
	if (nullptr == _itemBagData)
		return;

	MakeShared<FGsBMShopOpenPopupGroupDetail>(_itemBagData->itemId)->Send();
}

void UGsUIBMShopProductItem::SetDetailBtnVisibility(const bool InFlag)
{
	_btnDetail->SetVisibility(true == InFlag ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIBMShopProductItem::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}