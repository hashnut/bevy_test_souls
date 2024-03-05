//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildStorageEntryAcquireDetail.h"
//#include "Components/TextBlock.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Guild/GsGuildHelper.h"
//#include "Item/GsItem.h"
//#include "Item/GsItemEquipment.h"
//#include "Management/GsMessageHolder.h"
//#include "Message/GsMessageContents.h"
//#include "Message/MessageParam/GsItemContentsMessageParam.h"
//#include "Shared/Client/SharedEnums/SharedItemEnum.h"
//#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
//#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
//#include "UI/UIContent/Tray/Guild/GsGuildStorageAcquireDetailListItem.h"
//#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
//#include "UTIL/GsTimeSyncUtil.h"
//
//
//void UGsUIGuildStorageEntryAcquireDetail::NativeDestruct()
//{
//	if (nullptr != _iconSelector)
//	{
//		_iconSelector->RemoveAllChildren();
//	}
//
//	_uiIcon = nullptr;
//	_item = nullptr;
//
//	Super::NativeDestruct();
//}
//
//void UGsUIGuildStorageEntryAcquireDetail::NativeOnListItemObjectSet(UObject* ListItemObject)
//{
//	UGsGuildStorageAcquireDetailListItem* listItem = Cast<UGsGuildStorageAcquireDetailListItem>(ListItemObject);
//	if (nullptr == listItem)
//		return;
//
//	FGsGuildStorageAcquireDetailData* acquireDetail = listItem->GetData();
//	if (nullptr == acquireDetail)
//		return;
//
//	// 아이템 아이콘 설정
//	InvalidateIcon(acquireDetail->_acquiredItem);
//
//	// 획득 내역 설정
//	FText findText;
//	FGsGuildHelper::GetGuildItemAcquireTypeText(acquireDetail->_itemAcquireType, findText);
//	_textBlockHistory->SetText(findText);
//
//	// 획득 시간 설정
//	FDateTime acquiredTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(acquireDetail->_acquiredTime);
//	_textBlockTime->SetText(FText::FromString(acquiredTime.ToString(TEXT("%Y.%m.%d"))));
//}
//
//void UGsUIGuildStorageEntryAcquireDetail::InvalidateIcon(FGsItem* InItem)
//{
//	if (nullptr == _uiIcon)
//	{
//		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
//		_iconSelector->AddChild(_uiIcon);
//	}
//
//	if (nullptr == _uiIcon)
//		return;
//
//	if (nullptr == InItem)
//		return;
//
//	_item = InItem;
//
//	if (ItemType::EQUIP == InItem->GetType())
//	{
//		const FGsItemEquipment* equipData = InItem->GetCastItem<FGsItemEquipment>();
//		if (nullptr == equipData)
//			return;
//
//		_uiIcon->SetNotOwnItem(equipData->GetTID(), equipData->GetAmount(), equipData->GetLevel(), equipData->GetEnchantShieldCount(), equipData->GetEnchantBonusIndex());
//	}
//	else
//	{
//		_uiIcon->SetNotOwnItem(InItem->GetTID(), InItem->GetAmount(), InItem->GetLevel());
//	}
//
//	_uiIcon->SetItemNameVisibility(false);
//
//	_uiIcon->SetRedDot(InItem->IsNew());
//	
//	if (_uiIcon->OnClickSlot.IsBound())
//	{
//		_uiIcon->OnClickSlot.Clear();
//	}
//	_uiIcon->OnClickSlot.AddUObject(this, &UGsUIGuildStorageEntryAcquireDetail::OnClickItemIcon);
//
//	if (_uiIcon->OnFinishedLongPressEvent.IsBound())
//	{
//		_uiIcon->OnFinishedLongPressEvent.Clear();
//	}
//	_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildStorageEntryAcquireDetail::OnLongPressIcon);
//}
//
//void UGsUIGuildStorageEntryAcquireDetail::InvalidateIcon()
//{
//	if (nullptr == _uiIcon)
//		return;
//
//	if (nullptr == _item)
//		return;
//
//	if (ItemType::EQUIP == _item->GetType())
//	{
//		const FGsItemEquipment* equipData = _item->GetCastItem<FGsItemEquipment>();
//		if (nullptr == equipData)
//			return;
//
//		_uiIcon->SetNotOwnItem(equipData->GetTID(), equipData->GetAmount(), equipData->GetLevel(), equipData->GetEnchantShieldCount(), equipData->GetEnchantBonusIndex());
//	}
//	else
//	{
//		_uiIcon->SetNotOwnItem(_item->GetTID(), _item->GetAmount(), _item->GetLevel());
//	}
//
//	_uiIcon->SetItemNameVisibility(false);
//
//	_uiIcon->SetRedDot(_item->IsNew());
//
//	if (_uiIcon->OnClickSlot.IsBound())
//	{
//		_uiIcon->OnClickSlot.Clear();
//	}
//	_uiIcon->OnClickSlot.AddUObject(this, &UGsUIGuildStorageEntryAcquireDetail::OnClickItemIcon);
//
//	if (_uiIcon->OnFinishedLongPressEvent.IsBound())
//	{
//		_uiIcon->OnFinishedLongPressEvent.Clear();
//	}
//	_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildStorageEntryAcquireDetail::OnLongPressIcon);
//}
//
//void UGsUIGuildStorageEntryAcquireDetail::OnClickItemIcon(UGsUIIconItem& InIcon)
//{
//	_item->SetIsNew(false);
//}
//
//void UGsUIGuildStorageEntryAcquireDetail::OnLongPressIcon(UGsUIIconItem& InIcon)
//{
//	_uiIcon->SetRedDot(false);
//	FGsItem* itemData = _uiIcon->GetItemIconData();
//	
//	FGsItemDetailOpenMessageParam param(itemData, true, true, UGsUIPopupItemDetailMain::CenterPosType::GuildStoragePos);
//	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
//}
//
