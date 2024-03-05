// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/SelectItemBox/GsUISelectItemEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Item/GsItem.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUISelectItemEntry::BeginDestroy()
{
	if (nullptr != _itemiconSelector)
	{
		_itemiconSelector = nullptr;
	}


	Super::BeginDestroy();
}

void UGsUISelectItemEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSelect->OnClicked.AddDynamic(this, &UGsUISelectItemEntry::OnClick);

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_itemiconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUISelectItemEntry::OnRefreshItemIcon);
	}
}

void UGsUISelectItemEntry::OnShow()
{
	_selectedImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISelectItemEntry::OnHide()
{
	if (OnSelectIndexDelegate.IsBound())
	{
		OnSelectIndexDelegate.Clear();
	}

	_iconSlotHelper->Clear();
	_itemBagData = nullptr;
	_index = 0;
}

void UGsUISelectItemEntry::SetData(const uint8 InIndex, const FGsSchemaRewardItemBagData* InBagData)
{
	_index = InIndex;
	_itemBagData = InBagData;
	_iconSlotHelper->RefreshAll(1);

	if (CurrencyType::NONE != _itemBagData->currencyType)
	{
		FText nameText = FGsCurrencyHelper::GetCurrencyLocalizedText(_itemBagData->currencyType);
		_itemNameText->SetText(nameText);
	}
	else
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(_itemBagData->itemId);
		if (nullptr != itemData)
		{
			_itemNameText->SetText(itemData->name);
		}
	}
}

void UGsUISelectItemEntry::OnClick()
{
	if (OnSelectIndexDelegate.IsBound())
	{
		OnSelectIndexDelegate.Broadcast(_index);
	}
}

void UGsUISelectItemEntry::SetSelectedImage(bool InState)
{
	_selectedImage->SetVisibility(true == InState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUISelectItemEntry::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (nullptr == _itemBagData)
	{
		itemIcon->SetNotOwnItem(0, 0);
		itemIcon->SetItemNameToolTipFlag(false);
		itemIcon->SetActiveIconBg(true);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUISelectItemEntry::OnLongPressIcon);
	}
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);

	if (CurrencyType::NONE == _itemBagData->currencyType)
	{
		itemIcon->SetNotOwnItem(_itemBagData->itemId, 1, _itemBagData->itemLevel);
		itemIcon->SetItemCountMinMax(_itemBagData->itemCountMin, _itemBagData->itemCountMax);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
	}
	else
	{
		itemIcon->SetCurrency(_itemBagData->currencyType, 1);
		itemIcon->SetItemCountMinMax(_itemBagData->currencyAmountMin, _itemBagData->currencyAmountMax);
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameToolTipFlag(false);
		itemIcon->SetItemNameVisibility(false);
	}
}

void UGsUISelectItemEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}