// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUINpcShopAutoSellListEntry.h"
#include "T1Project.h"
#include "Item/GsItemManager.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"


void UGsUINpcShopAutoSellListEntry::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUINpcShopAutoSellListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUINpcShopAutoSellListEntry::OnRefreshIconWrapBox);
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUINpcShopAutoSellListEntry::OnCreateIconWrapBox);
	}

	_btnSelect->OnClicked.AddDynamic(this, &UGsUINpcShopAutoSellListEntry::OnClickSelect);
}

void UGsUINpcShopAutoSellListEntry::OnShow()
{
	_selectedImage->SetVisibility(ESlateVisibility::Hidden);

	_isSelected = false;
	_displayItemTID = 0;
	
	_isEquipmentSlot = false;
	_itemCategoryMain = ItemCategoryMain::NONE;
	_itemGrade = ItemGrade::NONE;
}

void UGsUINpcShopAutoSellListEntry::SetDisplayItem(int32 InItemTID, const bool InIsSelected)
{
	const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == tableData)
		return;

	_iconSelector->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_iconSwitcher->SetVisibility(ESlateVisibility::Hidden);

	_isEquipmentSlot = false;
	_itemCategoryMain = ItemCategoryMain::NONE;
	_itemGrade = ItemGrade::NONE;

	_displayItemTID = InItemTID;

	_itemNameText->SetText(tableData->name);
	_iconSlotHelper->RefreshAll(1);

	SetSlotSelect(InIsSelected);
}

void UGsUINpcShopAutoSellListEntry::SetDisplayItem(const NpcShopAutoSellEquipmentType InType, const bool InIsSelected)
{
	_displayItemTID = 0;
	_isEquipmentSlot = true;
	_iconSelector->SetVisibility(ESlateVisibility::Hidden);
	_iconSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FString keyString;
	switch (InType)
	{
	case NpcShopAutoSellEquipmentType::Weapon_NormalGrade:
		_iconSwitcher->SetActiveWidgetIndex(3);
		keyString = "NormalWeapon";
		_itemCategoryMain = ItemCategoryMain::WEAPON;
		_itemGrade = ItemGrade::NORMAL;
		break;
	case NpcShopAutoSellEquipmentType::Weapon_MagicGrade:
		_iconSwitcher->SetActiveWidgetIndex(4);
		keyString = "MagicWeapon";
		_itemCategoryMain = ItemCategoryMain::WEAPON;
		_itemGrade = ItemGrade::MAGIC;
		break;
	case NpcShopAutoSellEquipmentType::Weapon_RareGrade:
		_iconSwitcher->SetActiveWidgetIndex(5);
		keyString = "RareWeapon";
		_itemCategoryMain = ItemCategoryMain::WEAPON;
		_itemGrade = ItemGrade::RARE;
		break;
	case NpcShopAutoSellEquipmentType::Armor_NormalGrade:
		_iconSwitcher->SetActiveWidgetIndex(0);
		keyString = "NormalArmor";
		_itemCategoryMain = ItemCategoryMain::ARMOR;
		_itemGrade = ItemGrade::NORMAL;
		break;
	case NpcShopAutoSellEquipmentType::Armor_MagicGrade:
		_iconSwitcher->SetActiveWidgetIndex(1);
		keyString = "MagicArmor";
		_itemCategoryMain = ItemCategoryMain::ARMOR;
		_itemGrade = ItemGrade::MAGIC;
		break;
	case NpcShopAutoSellEquipmentType::Armor_RareGrade:
		_iconSwitcher->SetActiveWidgetIndex(2);
		keyString = "RareArmor";
		_itemCategoryMain = ItemCategoryMain::ARMOR;
		_itemGrade = ItemGrade::RARE;
		break;
	case NpcShopAutoSellEquipmentType::Accessory_NormalGrade:
		_iconSwitcher->SetActiveWidgetIndex(6);
		keyString = "NormalAccessory";
		_itemCategoryMain = ItemCategoryMain::ACCESSORY;
		_itemGrade = ItemGrade::NORMAL;
		break;
	case NpcShopAutoSellEquipmentType::Accessory_MagicGrade:
		_iconSwitcher->SetActiveWidgetIndex(7);
		keyString = "MagicAccessory";
		_itemCategoryMain = ItemCategoryMain::ACCESSORY;
		_itemGrade = ItemGrade::MAGIC;
		break;
	case NpcShopAutoSellEquipmentType::Accessory_RareGrade:
		_iconSwitcher->SetActiveWidgetIndex(8);
		keyString = "RareAccessory";
		_itemCategoryMain = ItemCategoryMain::ACCESSORY;
		_itemGrade = ItemGrade::RARE;
		break;
	}

	FText findText;
	FText::FindText(TEXT("NpcShopText"), keyString, findText);
	_itemNameText->SetText(findText);

	SetSlotSelect(InIsSelected);
	//GSLOG(Warning, TEXT("[UGsUINpcShopAutoSellListEntry::SetDisplayItem] - InIsSelected : %d"), (uint8)InIsSelected);
}

void UGsUINpcShopAutoSellListEntry::OnClickSelect()
{
	//GSLOG(Warning, TEXT("UGsUINpcShopAutoSellListEntry::OnClickSelect()"));

	_isSelected = _isSelected == true ? false : true;
	_selectedImage->SetVisibility(_isSelected == true ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUINpcShopAutoSellListEntry::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetNotOwnItem(_displayItemTID, 1);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUINpcShopAutoSellListEntry::OnClickIconSlot);
	}
}

void UGsUINpcShopAutoSellListEntry::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemCount(0);
	itemIcon->SetCoolTimeBg(false);
}

void UGsUINpcShopAutoSellListEntry::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = false;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

ItemCategoryMain UGsUINpcShopAutoSellListEntry::GetSlotItemCategory()
{
	return _itemCategoryMain;
}

ItemGrade UGsUINpcShopAutoSellListEntry::GetSlotItemGrade()
{
	return _itemGrade;
}

void UGsUINpcShopAutoSellListEntry::SetSlotSelect(bool InState)
{
	_isSelected = InState;
	_selectedImage->SetVisibility(_isSelected == true ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

