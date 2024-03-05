// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPlayerEquipInfo.h"

#include "Animation/WidgetAnimation.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"


void UGsUIPlayerEquipInfo::BeginDestroy()
{
	if (_btnEquipFirst)
	{
		_btnEquipFirst->OnClicked.RemoveDynamic(this, &UGsUIPlayerEquipInfo::OnClickPageFirst);
	}
	if (_btnEquipSecond)
	{
		_btnEquipSecond->OnClicked.RemoveDynamic(this, &UGsUIPlayerEquipInfo::OnClickPageSecond);
	}

	for (int32 i = 0;i < _iconSlotHelperList.Num();++i)
	{
		if (nullptr != _iconSlotHelperList[i])
		{
			_iconSlotHelperList[i]->OnRefreshIcon.RemoveDynamic(this, &UGsUIPlayerEquipInfo::OnRefreshIcon);
		}
	}
	_iconSlotHelperList.Empty();

	Super::BeginDestroy();
}

void UGsUIPlayerEquipInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnEquipFirst->OnClicked.AddDynamic(this, &UGsUIPlayerEquipInfo::OnClickPageFirst);
	_btnEquipSecond->OnClicked.AddDynamic(this, &UGsUIPlayerEquipInfo::OnClickPageSecond);

	_iconSlotHelperList.Empty();
	FindIconSelecter(_panelIconRootLeft, _iconSlotHelperList);
	FindIconSelecter(_panelIconRootRight, _iconSlotHelperList);
	FindIconSelecter(_panelIconRootBMLeft, _iconSlotHelperList);
	FindIconSelecter(_panelIconRootBMRight, _iconSlotHelperList);
}

void UGsUIPlayerEquipInfo::NativeConstruct()
{
	Super::NativeConstruct();

	// 순서 유의
	_equipSlotList.Empty();
	_equipSlotList.Emplace(EGsEquipSlotType::HELMET_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::ARMOR_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SHOES_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::NECKLACE_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::EAR_RING_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::RING_LEFT_TYPE);

	_equipSlotList.Emplace(EGsEquipSlotType::WEAPON_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::BELT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::GLOVE_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SCARF_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::EAR_RING_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::RING_RIGHT_TYPE);

	_equipSlotList.Emplace(EGsEquipSlotType::CHARM_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::BRACELET_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::DURAL_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::MARBLE_LEFT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);

	_equipSlotList.Emplace(EGsEquipSlotType::CHARM_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::BRACELET_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotList.Emplace(EGsEquipSlotType::MARBLE_RIGHT_TYPE);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);
	_equipSlotList.Emplace(EGsEquipSlotType::SLOT_MAX);

	_btnEquipFirst->SetSwitcherIndex(0);
	_btnEquipSecond->SetSwitcherIndex(1);

	PlayAnimation(_animMoveToFirst, _animMoveToFirst->GetEndTime() - 0.1f);
}

void UGsUIPlayerEquipInfo::NativeDestruct()
{
	_equipSlotList.Empty();

	Super::NativeDestruct();
}

void UGsUIPlayerEquipInfo::FindIconSelecter(UPanelWidget* InPanel, OUT TArray<UGsDynamicIconSlotHelper*>& OutIconSlotHelperList)
{
	for (int32 i = 0, childCount = InPanel->GetChildrenCount(); i < childCount; ++i)
	{
		UWidget* childWidget = InPanel->GetChildAt(i);
		if (childWidget->IsA<UGsHorizontalBoxIconSelector>())
		{
			UGsDynamicIconSlotHelper* iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
			iconSlotHelper->Initialize(Cast<UGsHorizontalBoxIconSelector>(childWidget));
			iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPlayerEquipInfo::OnRefreshIcon);
			OutIconSlotHelperList.Emplace(iconSlotHelper);
		}
	}
}

void UGsUIPlayerEquipInfo::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	EGsEquipSlotType slotType = _refreshSlotType;
	_refreshSlotType = EGsEquipSlotType::SLOT_MAX;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	do 
	{
		UGsItemManager* itemManager = GItem();
		if (nullptr == itemManager)
			break;

		uint64 itemDBID = itemManager->GetEquipedItemDBIDBySlotType(slotType);
		if (INVALID_ITEM_DB_ID == itemDBID)
			break;

		UGsGameUserSettings* userSetting = GGameUserSettings();
		if (nullptr == userSetting)
			break;

		int32 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
		bool isUseLongPress = (0 == value) ? false : true;

		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPlayerEquipInfo::OnLongPressIcon);
		}

		if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
		{
			itemIcon->OnClickSlot.AddUObject(this, &UGsUIPlayerEquipInfo::OnClickIcon);
		}

		itemIcon->SetOwnedItem(itemDBID);
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetSlotEquipedImage(false);
		itemIcon->SetPresetImage(false);
		itemIcon->SetRedDot(false);
		itemIcon->SetItemNameToolTipHoverMode(true);
		itemIcon->SetItemNameToolTipFlag(isUseLongPress);
		itemIcon->SetOnChangeDisplayDetailWndType(true == isUseLongPress ?
			UGsUIIconItem::UseDisplayDetailWndType::LongPressType : UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
		return;
	} while (0);
	
	itemIcon->SetEmptyIcon();
	itemIcon->SetActiveIconBg(false);
}

void UGsUIPlayerEquipInfo::OnClickIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr == userSetting)
		return;

	uint8 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
	bool isUseLongPress = 0 == value ? false : true;
	if (false == isUseLongPress)
	{
		// 아이템 상세창 open
		FGsItem* item = itemIcon->GetItemIconData();
		FGsItemDetailOpenMessageParam param(item, false, true, UGsUIPopupItemDetailMain::CenterPosType::LeftPos);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_CHAR_DETAIL, &param);
	}
}

void UGsUIPlayerEquipInfo::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	// 아이템 상세창 open
	FGsItem* item = itemIcon->GetItemIconData();
	FGsItemDetailOpenMessageParam param(item, false, true, UGsUIPopupItemDetailMain::CenterPosType::LeftPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_CHAR_DETAIL, &param);
}

void UGsUIPlayerEquipInfo::OnClickPageFirst()
{
	if (_btnEquipFirst->GetSwitcherIndex() == 1)
	{
		_btnEquipFirst->SetSwitcherIndex(0);
		_btnEquipSecond->SetSwitcherIndex(1);

		PlayAnimation(_animMoveToFirst);
	}
}

void UGsUIPlayerEquipInfo::OnClickPageSecond()
{
	if (_btnEquipFirst->GetSwitcherIndex() == 0)
	{
		_btnEquipFirst->SetSwitcherIndex(1);
		_btnEquipSecond->SetSwitcherIndex(0);

		PlayAnimation(_animMoveToSecond);
	}
}

void UGsUIPlayerEquipInfo::InvalidateSlotAll()
{
	for (int32 i = 0;i < _iconSlotHelperList.Num();++i)
	{
		if (nullptr != _iconSlotHelperList[i])
		{
			_refreshSlotType = _equipSlotList[i];
			_iconSlotHelperList[i]->RefreshAll(1);
		}
	}
}

void UGsUIPlayerEquipInfo::InvalidateSlot(EGsEquipSlotType InSlotType)
{
	if (InSlotType == EGsEquipSlotType::SLOT_MAX)
		return;

	int32 index = _equipSlotList.IndexOfByKey(InSlotType);
	if (_iconSlotHelperList.IsValidIndex(index))
	{
		if (nullptr != _iconSlotHelperList[index])
		{
			_refreshSlotType = _equipSlotList[index];
			_iconSlotHelperList[index]->RefreshAll(1);
		}
	}
}