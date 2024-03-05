// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/Mail/GsUIItemReceiveEntry.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/GsSchemaCurrencyData.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "UTIL/GsTableUtil.h"

#include "Currency/GsCurrencyHelper.h"



void UGsUIItemReceiveEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIItemReceiveEntry::NativeDestruct()
{
	if (_iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

UGsUIIconItemReward* UGsUIItemReceiveEntry::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItemReward>(_iconSelector->GetChildAt(0));
}

// 아이콘 상세보기 창
void UGsUIItemReceiveEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIItemReceiveEntry::SetCurrencyData(CurrencyType InType, Currency InAmount)
{
	_nameText->SetText(GetCurrencyName(InType));
	// 아이콘
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);

		itemIcon->SetCurrency(InType, InAmount);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIItemReceiveEntry::OnLongPressIcon);
	}
}

void UGsUIItemReceiveEntry::SetItemData(const uint32 InTID, const ItemAmount InAmount)
{
	_nameText->SetText(GetItemName(InTID));
	
	// 아이콘
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);

		itemIcon->SetNotOwnItem(InTID, InAmount);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIItemReceiveEntry::OnLongPressIcon);
	}
}

void UGsUIItemReceiveEntry::SetEquipItemData(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel, const uint8 InEnchantShieldCount, const uint8 InEnchantBonusIndex)
{
	_nameText->SetText(GetItemName(InTID));
	// 아이콘
	if (UGsUIIconItemReward* itemIcon = GetIcon())
	{
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);

		itemIcon->SetNotOwnItem(InTID, InAmount, InEnchantLevel, InEnchantShieldCount, InEnchantBonusIndex);
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIItemReceiveEntry::OnLongPressIcon);
	}
}

const FText& UGsUIItemReceiveEntry::GetItemName(const uint32 InTID) const
{
	const FGsSchemaItemCommon* SchemaItemCommon =
		UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(InTID);
	if (nullptr != SchemaItemCommon)
	{
		return SchemaItemCommon->name;
	}

	return FText::GetEmpty();
}

FText UGsUIItemReceiveEntry::GetCurrencyName(CurrencyType InType) const
{
	return FGsCurrencyHelper::GetCurrencyLocalizedText(InType);
}