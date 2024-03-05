// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "T1Project.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"
#include "UTIL/GsItemUtil.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetail.h"

//#include "Components/Spacer.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

void UGsUIPopupItemDetailMain::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _itemDetailWbp->OnGetItemDataDelegate.IsBoundToObject(this))
	{
		_itemDetailWbp->OnGetItemDataDelegate.BindUObject(this, &UGsUIPopupItemDetailMain::FindItemData);
	}

	if (ESlateVisibility::Hidden == _itemDetailWbp->GetVisibility())
	{
		_itemDetailWbp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	SetCenterPos(CenterPosType::RightPos);
	SetbgImg(false);

	if (false == _itemDetailWbp->OnCloseItemDetailDelegate.IsBoundToObject(this))
	{
		_itemDetailWbp->OnCloseItemDetailDelegate.AddUObject(this, &UGsUIPopupItemDetailMain::CloseItemDetailUI);
	}
}

void UGsUIPopupItemDetailMain::NativeDestruct()
{
	//FGsResetInvenSelectedIcon invenParam(_keepSelectionFlag);
	//GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::ResetSelectedIcon, invenParam);

	if (OnCloseItemDetailMainDelegate.IsBound())
	{
		OnCloseItemDetailMainDelegate.Clear();
	}

	if (_notOwnedItemPtr.IsValid())
	{
		_notOwnedItemPtr.Reset();
	}

	_keepSelectionFlag = false;
	_itemDbid = 0;

	Super::NativeDestruct();
}

void UGsUIPopupItemDetailMain::SetItemData(const FGsItem* InItem, bool InHideButtons, CenterPosType InCenterPosType, bool InHideBG, bool InHideButtonsBottom, bool InUseLock, bool InUseItemCollectionMarking)
{
	if (nullptr == InItem)
		return;

	if (0 < InItem->GetDBID())
	{
		_itemDbid = InItem->GetDBID();
	}
	else
	{
		_notOwnedItemPtr.Reset();
		_notOwnedItemPtr = MakeShareable(FGsItem::Create(InItem->GetTID()));
		if (_notOwnedItemPtr.IsValid())
		{
			_notOwnedItemPtr->UpdateBy(InItem);
		}
	}
	_itemDetailWbp->SetItemDefaultData(InItem, InHideButtons, UGsUIPopupItemDetail::DisplayModeType::ItemDetailMode, InHideButtonsBottom, InUseLock, InUseItemCollectionMarking);
	_itemDetailWbp->SetItemDetailData(InItem, InUseItemCollectionMarking);
	
	SetCenterPos(InCenterPosType);
	SetbgImg(InHideBG);
}

void UGsUIPopupItemDetailMain::SetItemData(const uint32 InItemTID, bool InHideButtons, CenterPosType InCenterPosType, bool InHideBG)
{
	_notOwnedItemPtr.Reset();
	_notOwnedItemPtr = MakeShareable(FGsItem::Create(InItemTID));
	if (_notOwnedItemPtr.IsValid())
	{
		_itemDetailWbp->SetItemDefaultData(InHideButtons);
		_itemDetailWbp->SetItemDetailData();

		SetCenterPos(InCenterPosType);
		SetbgImg(InHideBG);
	}
}

void UGsUIPopupItemDetailMain::SetItemDataItemCollectionConditionItemOnly(const FGsItem* InItem, bool InShowBtn, bool InEnchant, bool InChanged, bool InRegister, bool InIsBtnRegisterType)
{
	if (nullptr == InItem)
		return;

	if (0 < InItem->GetDBID())
	{
		_itemDbid = InItem->GetDBID();
	}
	else
	{
		_notOwnedItemPtr.Reset();
		_notOwnedItemPtr = MakeShareable(FGsItem::Create(InItem->GetTID()));
		if (_notOwnedItemPtr.IsValid())
		{
			_notOwnedItemPtr->UpdateBy(InItem);
		}
	}

	_itemDetailWbp->SetItemCollectionConditionItemOnly(InShowBtn, InEnchant, InChanged, InRegister, InIsBtnRegisterType);
	_itemDetailWbp->SetItemDetailData();

	SetCenterPos(CenterPosType::CenterPos);
	SetbgImg(false);
}

void UGsUIPopupItemDetailMain::SetItemCollectionRegisterInfoText(const FText& InText)
{
	_itemDetailWbp->SetItemCollectionRegisterInfoText(InText);
}

void UGsUIPopupItemDetailMain::SetCurrencyData(const CurrencyType InCurrencyType, const Currency InAmount, bool InHideButtons, const CenterPosType InCenterPosType, bool InHideBG)
{
	_itemDetailWbp->SetCurrencyData(InCurrencyType, InAmount, InHideButtons);

	SetCenterPos(InCenterPosType);
	SetbgImg(InHideBG);
}

void UGsUIPopupItemDetailMain::UpdateItemData(const uint64 InItemDBID)
{
	_itemDetailWbp->UpdateItemData(InItemDBID);
}

void UGsUIPopupItemDetailMain::RemoveItemData(const uint64 InItemDBID)
{
	if (true == _itemDetailWbp->IsRemoveItem(InItemDBID))
	{
		Close();
	}
}

void UGsUIPopupItemDetailMain::CloseItemDetailUI()
{
	if (OnCloseItemDetailMainDelegate.IsBound())
	{
		OnCloseItemDetailMainDelegate.Broadcast();
		OnCloseItemDetailMainDelegate.Clear();
	}
	if (_notOwnedItemPtr.IsValid())
	{
		_notOwnedItemPtr.Reset();
	}
	_itemDbid = 0;

	Close();
}

void UGsUIPopupItemDetailMain::SetCenterPos(CenterPosType InCenterPosType)
{
	switch (InCenterPosType)
	{
	case CenterPosType::CenterPos:
		_centerAlignSpacerLeft->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_centerAlignSpacer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_sizeBoxGuildStorageList->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case CenterPosType::LeftPos:
		_centerAlignSpacerLeft->SetVisibility(ESlateVisibility::Collapsed);
		_centerAlignSpacer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_sizeBoxGuildStorageList->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case CenterPosType::RightPos:
		_centerAlignSpacerLeft->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_centerAlignSpacer->SetVisibility(ESlateVisibility::Collapsed);
		_sizeBoxGuildStorageList->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case CenterPosType::GuildStoragePos:
		_centerAlignSpacerLeft->SetVisibility(ESlateVisibility::Collapsed);
		_centerAlignSpacer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_sizeBoxGuildStorageList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIPopupItemDetailMain::SetbgImg(bool InHideBG)
{
	_bgImg->SetVisibility(InHideBG ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

const FGsItem* UGsUIPopupItemDetailMain::FindItemData()
{
	const FGsItem* findItemData = UGsItemUtil::FindItemUtil(_itemDbid);
	if (nullptr == findItemData)
	{
		if (_notOwnedItemPtr.IsValid())
		{
			findItemData = _notOwnedItemPtr.Get();
		}
	}
	return findItemData;
}

void UGsUIPopupItemDetailMain::OnInputCancel()
{
	_itemDetailWbp->OnClickClose();
}

