// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Common/Item/GsUIItemSupplyIconEntry.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UMG/Public/Components/TextBlock.h"
#include "T1Project.h"

void UGsUIItemSupplyIconEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIItemSupplyIconEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIItemSupplyIconEntry::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
		_iconItemInventory = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIItemSupplyIconEntry::NativeOnListItemObjectSet(UObject* InItemData)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	UGsUIItemSupplyIconItem* itemData = Cast<UGsUIItemSupplyIconItem>(InItemData);
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	if (nullptr == _iconItemInventory)
	{
		_iconItemInventory = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_iconItemInventory);
	}

	if (nullptr == _iconItemInventory)
	{
		GSLOG(Error, TEXT("nullptr == _iconItemInventory, GetName():%s"), _iconItemInventory, *GetName());
		return;
	}

	UGsUIItemSupplyIconItem::Parameters _parameters = itemData->GetParameters();

	if (CostType::CURRENCY == _parameters._costType)
	{
		_iconItemInventory->SetCurrency(_parameters._currencyType, _parameters._amountMin, _parameters._amountMax);
		_iconItemInventory->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
	}
	else if (CostType::ITEM == _parameters._costType)
	{
		_iconItemInventory->SetNotOwnItem(_parameters._itemId, _parameters._amountMin, _parameters._itemLevel);
		_iconItemInventory->SetItemCountMinMax(_parameters._amountMin, _parameters._amountMax);

		if (false == _iconItemInventory->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			_iconItemInventory->OnFinishedLongPressEvent.Clear();
			_iconItemInventory->OnFinishedLongPressEvent.AddUObject(this, &UGsUIItemSupplyIconEntry::OnLongPressIcon);
		}
		_iconItemInventory->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	}
	else
	{
		GSLOG(Error, TEXT("CostType should be CURRENCY or ITEM!"));
		return;
	}

	_iconItemInventory->SetActiveIconBg(true);
	_iconItemInventory->SetItemNameVisibility(false);
	_iconItemInventory->SetDimmedImage(false);
	_iconItemInventory->SetSlotSelectionImage(false);
	_iconItemInventory->SetItemNameToolTipFlag(true);
	_iconItemInventory->SetItemNameToolTipHoverMode(true);
	_iconItemInventory->SetRedDot(false);

	IUserObjectListEntry::NativeOnListItemObjectSet(InItemData);
}

void UGsUIItemSupplyIconEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItem* item = itemIcon->GetItemIconData();
	if (nullptr == item)
		return;

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = true;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = false;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::RightPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_BOX, &param);
}
