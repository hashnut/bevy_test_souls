//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildStorageEntryDistributionDetail.h"
//#include "Components/TextBlock.h"
//#include "Components/WidgetSwitcher.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Item/GsItem.h"
//#include "Item/GsItemEquipment.h"
//#include "Management/GsMessageHolder.h"
//#include "Message/GsMessageContents.h"
//#include "Message/MessageParam/GsItemContentsMessageParam.h"
//#include "Shared/Client/SharedEnums/SharedItemEnum.h"
//#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
//#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
//#include "UI/UIContent/Tray/Guild/GsGuildStorageDistributionDetailListItem.h"
//#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
//#include "UTIL/GsTimeSyncUtil.h"
//
//
//void UGsUIGuildStorageEntryDistributionDetail::NativeDestruct()
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
//void UGsUIGuildStorageEntryDistributionDetail::NativeOnListItemObjectSet(UObject* ListItemObject)
//{
//	UGsGuildStorageDistributionDetailListItem* listItem = Cast<UGsGuildStorageDistributionDetailListItem>(ListItemObject);
//	if (nullptr == listItem)
//		return;
//
//	FGsGuildStorageDistributionDetailData* distributionDetail = listItem->GetData();
//	if (nullptr == distributionDetail)
//		return;
//
//	// 아이템 아이콘 설정
//	InvalidateIcon(distributionDetail->_distributedItem);
//
//	// 분배를 주고 받은 사람이 없으면 기간 만료에 따른 삭제
//	if (true == distributionDetail->_giverName.IsEmpty() /* && true == distributionDetail->_receiverName.IsEmpty()*/)
//	{
//		_switcherGiver->SetActiveWidgetIndex(1);
//		_switcherReceiver->SetActiveWidgetIndex(1);
//	}
//	else
//	{
//		_switcherGiver->SetActiveWidgetIndex(0);
//		_switcherReceiver->SetActiveWidgetIndex(0);
//
//		_textBlockGiver->SetText(FText::FromString(distributionDetail->_giverName));
//		_textBlockReceiver->SetText(FText::FromString(distributionDetail->_receiverName));
//	}
//	
//	// 분배 시간 설정
//	FDateTime distributedTime = FGsTimeSyncUtil::ConvertToDateTime(distributionDetail->_distributedTime);
//	_textBlockTime->SetText(FText::FromString(distributedTime.ToString(TEXT("%Y.%m.%d"))));
//}
//
//void UGsUIGuildStorageEntryDistributionDetail::InvalidateIcon(FGsItem* InItem)
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
//	_uiIcon->OnClickSlot.AddUObject(this, &UGsUIGuildStorageEntryDistributionDetail::OnClickItemIcon);
//
//	if (_uiIcon->OnFinishedLongPressEvent.IsBound())
//	{
//		_uiIcon->OnFinishedLongPressEvent.Clear();
//	}
//	_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildStorageEntryDistributionDetail::OnLongPressIcon);
//}
//
//void UGsUIGuildStorageEntryDistributionDetail::InvalidateIcon()
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
//	_uiIcon->OnClickSlot.AddUObject(this, &UGsUIGuildStorageEntryDistributionDetail::OnClickItemIcon);
//
//	if (_uiIcon->OnFinishedLongPressEvent.IsBound())
//	{
//		_uiIcon->OnFinishedLongPressEvent.Clear();
//	}
//	_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildStorageEntryDistributionDetail::OnLongPressIcon);
//}
//
//void UGsUIGuildStorageEntryDistributionDetail::OnClickItemIcon(UGsUIIconItem& InIcon)
//{
//	_item->SetIsNew(false);
//}
//
//void UGsUIGuildStorageEntryDistributionDetail::OnLongPressIcon(UGsUIIconItem& InIcon)
//{
//	_uiIcon->SetRedDot(false);
//	FGsItem* item = _uiIcon->GetItemIconData();
//	
//	FGsItemDetailOpenMessageParam param(item, true, true, UGsUIPopupItemDetailMain::CenterPosType::GuildStoragePos);
//	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
//}
