// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupItemDetailCompare.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetail.h"
//#include "Components/Spacer.h"
#include "Components/Image.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"
#include "UTIL/GsItemUtil.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIPopupItemDetailCompare::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_bgImg->OnMouseButtonDownEvent.BindDynamic(this, &UGsUIPopupItemDetailCompare::OnPressBGImg);

	_mainItemDetail->OnChangeOtherItemEnchantCount.BindUObject(this, &UGsUIPopupItemDetailCompare::OnChangeMainItemEnchantCount);
	_subItemDetail->OnChangeOtherItemEnchantCount.BindUObject(this, &UGsUIPopupItemDetailCompare::OnChangeSubItemEnchantCount);
}

void UGsUIPopupItemDetailCompare::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _mainItemDetail->OnGetItemDataDelegate.IsBoundToObject(this))
	{
		_mainItemDetail->OnGetItemDataDelegate.BindUObject(this, &UGsUIPopupItemDetailCompare::FindMainItemData);
	}
	if (false == _subItemDetail->OnGetItemDataDelegate.IsBoundToObject(this))
	{
		_subItemDetail->OnGetItemDataDelegate.BindUObject(this, &UGsUIPopupItemDetailCompare::FindSubItemData);
	}
	if (false == _mainItemDetail->OnCloseItemDetailDelegate.IsBoundToObject(this))
	{
		_mainItemDetail->OnCloseItemDetailDelegate.AddUObject(this, &UGsUIPopupItemDetailCompare::CloseMainDetailUI);
	}
	if (false == _subItemDetail->OnCloseItemDetailDelegate.IsBoundToObject(this))
	{
		_subItemDetail->OnCloseItemDetailDelegate.AddUObject(this, &UGsUIPopupItemDetailCompare::CloseSubDetailUI);
	}
	if (false == _mainItemDetail->OnItemDetailCompareDelegate.IsBoundToObject(this))
	{
		_mainItemDetail->OnItemDetailCompareDelegate.AddUObject(this, &UGsUIPopupItemDetailCompare::OnClickCompareBtn);
	}
	if (false == _subItemDetail->OnItemDetailCompareNextDelegate.IsBoundToObject(this))
	{
		_subItemDetail->OnItemDetailCompareNextDelegate.AddUObject(this, &UGsUIPopupItemDetailCompare::OnCompareNextSlotItem);
	}

	SetCenterPos(false);
	SetbgImg(false);
}

void UGsUIPopupItemDetailCompare::NativeDestruct()
{
	if (_notOwnedItemPtr.IsValid())
	{
		_notOwnedItemPtr.Reset();
	}
	_mainItemDbid = 0;
	_subItemDbid = 0;

	Super::NativeDestruct();
}

void UGsUIPopupItemDetailCompare::CloseMainDetailUI()
{
	//FGsResetInvenSelectedIcon invenParam;
	//GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::ResetSelectedIcon, invenParam);

	Close();
}

void UGsUIPopupItemDetailCompare::CloseSubDetailUI()
{
	_subItemDetail->SetCompareState(false);
	_subItemDetail->SetHideVisiblity(false);
	_mainItemDetail->SetCompareValueVisibility(false);
}

void UGsUIPopupItemDetailCompare::OnClickCompareBtn()
{
	bool curState = _subItemDetail->GetCompareState();
	_subItemDetail->SetCompareState(!curState);
	_subItemDetail->SetHideVisiblity(!curState);
	_mainItemDetail->SetCompareValueVisibility(!curState);
}

// 메인 디테일은 랜덤(유저가 선택한 아이템 , 갖고있지 않을수도 있음, 타 유저 아이템(프로필))
// 서브 디테일은 내가 착용중인 아이템이 고정으로 나옴
void UGsUIPopupItemDetailCompare::SetItemData(FGsItem* InTargetItem, uint64 InOwnedItemDbid, bool InHideButtons, bool InCenterPopup, bool InHideBG, bool InHideButtonsBottom, bool InUseLock)
{
	if (0 >= InTargetItem->GetDBID()) // 어디에도 갖고있지 않는 아이템
	{
		_notOwnedItemPtr = MakeShareable(FGsItem::Create(InTargetItem->GetTID()));
		_notOwnedItemPtr->UpdateBy(InTargetItem);
	}
	else
	{
		_mainItemDbid = InTargetItem->GetDBID();
	}
	_subItemDbid = InOwnedItemDbid;
	
	_mainItemDetail->SetItemDefaultData(InHideButtons, UGsUIPopupItemDetail::DisplayModeType::ItemDetailMode, InHideButtonsBottom, InUseLock);
	_mainItemDetail->SetItemCompareDetailData(FindSubItemData());
	_mainItemDetail->SetCompareValueVisibility(true);

	_subItemDetail->SetItemDefaultData(InHideButtons, UGsUIPopupItemDetail::DisplayModeType::ItemCompareMode, false, InUseLock);
	_subItemDetail->SetItemDetailData();
	_subItemDetail->SetCompareState(true);
	_subItemDetail->SetHideVisiblity(true);

	SetCenterPos(InCenterPopup);
	SetbgImg(InHideBG);
}

void UGsUIPopupItemDetailCompare::SetCenterPos(bool InCenterPopup)
{
	_centerAlignSpacer->SetVisibility(false == InCenterPopup ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIPopupItemDetailCompare::SetbgImg(bool InHideBG)
{
	_bgImg->SetVisibility(InHideBG ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UGsUIPopupItemDetailCompare::UpdateItemData(const uint64 InItemDBID, bool InSkipUpdateDetailInfo)
{
	_mainItemDetail->UpdateItemData(InItemDBID, InSkipUpdateDetailInfo);
	_subItemDetail->UpdateItemData(InItemDBID, InSkipUpdateDetailInfo);
}

void UGsUIPopupItemDetailCompare::RemoveItemData(const uint64 InItemDBID)
{
	if (_mainItemDetail->IsRemoveItem(InItemDBID) || _subItemDetail->IsRemoveItem(InItemDBID))
	{
		CloseMainDetailUI();
	}
}

const FGsItem* UGsUIPopupItemDetailCompare::FindMainItemData()
{
	const FGsItem* findItemData = UGsItemUtil::FindItemUtil(_mainItemDbid);
	if (nullptr == findItemData)
	{
		if (_notOwnedItemPtr.IsValid())
		{
			findItemData = _notOwnedItemPtr.Get();
		}
	}
	return findItemData;
}

const FGsItem* UGsUIPopupItemDetailCompare::FindSubItemData()
{
	const FGsItem* findItemData = UGsItemUtil::FindItemUtil(_subItemDbid);
	return findItemData;
}

void UGsUIPopupItemDetailCompare::OnCompareNextSlotItem(const EGsEquipSlotType InTargetSlotType, const bool InHideBtnFlag, const bool InHideButtonsBottom, const bool InUseLock)
{
	//GSLOG(Warning, TEXT("[UGsUIPopupItemDetailCompare::OnCompareNextSlotItem] - InTargetSlotType : %d"), (uint8)InTargetSlotType);
	TWeakPtr<FGsItem> ownedItem = GItem()->GetPlayerEquipedItemBySlotType(InTargetSlotType);
	if (false == ownedItem.IsValid())
		return;

	_subItemDbid = ownedItem.Pin()->GetDBID();

	_mainItemDetail->SetItemCompareDetailData(FindSubItemData());

	_subItemDetail->SetItemDefaultData(InHideBtnFlag, UGsUIPopupItemDetail::DisplayModeType::ItemCompareMode, InHideButtonsBottom, InUseLock);
	_subItemDetail->SetItemDetailData();
}

void UGsUIPopupItemDetailCompare::OnChangeMainItemEnchantCount(int32 InCount)
{
	_subItemDetail->SetCompareItemEnchantCount(InCount);
}

void UGsUIPopupItemDetailCompare::OnChangeSubItemEnchantCount(int32 InCount)
{
	_mainItemDetail->SetCompareItemEnchantCount(InCount);
}

void UGsUIPopupItemDetailCompare::OnInputOk()
{
	OnClickCompareBtn();
}

void UGsUIPopupItemDetailCompare::OnInputCancel()
{
	//CloseSubDetailUI();
	CloseMainDetailUI();
}

FEventReply UGsUIPopupItemDetailCompare::OnPressBGImg(FGeometry InGeometry, const FPointerEvent& InMouseEvent)
{
	OnInputCancel();

	return FEventReply(false);
}