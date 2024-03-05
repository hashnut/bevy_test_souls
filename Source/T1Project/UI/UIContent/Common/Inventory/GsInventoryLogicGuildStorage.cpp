//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsInventoryLogicGuildStorage.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Management/GsMessageHolder.h"
//#include "Management/ScopeGlobal/GsOptionManager.h"
//#include "Management/ScopeGlobal/GsGuildManager.h"
//#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Message/MessageParam/GsItemContentsMessageParam.h"
//#include "Net/GsNetSendServiceGuild.h"
//#include "Item/GsItem.h"
//#include "Shared/Client/SharedEnums/SharedItemEnum.h"
//#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
//#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
//#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
//#include "UI/UIContent/Helper/GsUIHelper.h"
//#include "UTIL/GsTimeSyncUtil.h"
//#include "Currency/GsCostPackage.h"
//
//
//void FGsInventoryLogicGuildStorage::Clear()
//{
//	_dataList.Empty();
//	_selectedItemDBId = INVALID_ITEM_DB_ID;
//
//	Super::Clear();
//}
//
//void FGsInventoryLogicGuildStorage::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
//{
//	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
//	if (nullptr == itemIcon)
//		return;
//
//	if (false == _dataList.IsValidIndex(InIndex))
//	{
//		itemIcon->SetEmptyIcon();
//		return;
//	}
//
//	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
//	{
//		itemIcon->OnFinishedLongPressEvent.AddRaw(this, &FGsInventoryLogicGuildStorage::OnLongPressIcon);
//	}
//
//	if (FGsItem* itemData = _dataList[InIndex])
//	{
//		itemIcon->SetNotOwnItem(itemData->GetTID(), itemData->GetAmount());
//		FGsItem* item = itemIcon->GetItemIconData();
//		item->SetDBID(itemData->GetDBID());
//		item->SetIsNew(itemData->IsNew());
//		item->SetIsGuildStorageItem(true);
//		item->SetExpiredTime(itemData->GetExpiredTime());
//		itemIcon->SetRedDot(itemData->IsNew());
//		itemIcon->SetActiveIconBg(true);
//		itemIcon->SetItemNameVisibility(false);
//		itemIcon->SetItemNameToolTipFlag(true);
//		itemIcon->SetItemNameToolTipHoverMode(true);
//		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
//		itemIcon->SetSlotSelectionImage((itemData->GetDBID() == _selectedItemDBId) ? true : false);
//		itemIcon->SetExpiredTime(item);
//	}
//}
//
//void FGsInventoryLogicGuildStorage::OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon)
//{
//	// 이전 아이콘 취소 표시
//	if (InPrevIcon)
//	{
//		if (UGsUIIconItemInventory* prevIcon = Cast<UGsUIIconItemInventory>(InPrevIcon))
//		{
//			prevIcon->SetSlotSelectionImage(false);
//		}
//	}
//
//	// 현재 아이콘 선택 표시
//	if (InCurrIcon)
//	{
//		if (FGsItem* item = InCurrIcon->GetItemIconData())
//		{
//			if (UGsUIIconItemInventory* currIcon = Cast<UGsUIIconItemInventory>(InCurrIcon))
//			{
//				currIcon->SetSlotSelectionImage(true);
//			}
//
//			SetSelectedItemDBId(item->GetDBID());
//		}
//		else
//		{
//			SetSelectedItemDBId(INVALID_ITEM_DB_ID);
//		}
//	}
//}
//
//void FGsInventoryLogicGuildStorage::OnLongPressIcon(UGsUIIconItem& InIcon)
//{
//	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
//	if (nullptr == itemIcon)
//		return;
//
//	FGsItem* itemData = itemIcon->GetItemIconData();
//	FGsItem* itemRealData = nullptr;
//	for (uint8 i = 0; i < _dataList.Num(); ++i)
//	{
//		itemRealData = _dataList[i];
//		if (itemData->GetDBID() == itemRealData->GetDBID())
//		{
//			itemIcon->SetRedDot(false);
//			itemData->SetIsNew(false);
//			itemRealData->SetIsNew(false);
//		}
//	}
//
//	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), true, true, UGsUIPopupItemDetailMain::CenterPosType::RightPos);
//	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
//}
//
//void FGsInventoryLogicGuildStorage::OnNativeConstruct()
//{
//	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
//	{
//		_invenTotalSlotCount = storageData->_maxSlotCount;
//		_totalExtendCount = storageData->_slotExtendedCount;
//	}
//
//	_selectedItemDBId = INVALID_ITEM_DB_ID;
//}
//
//void FGsInventoryLogicGuildStorage::OnInvalidateList()
//{
//	_dataList.Empty();
//
//	GetItemList(_currTabType);
//}
//
//void FGsInventoryLogicGuildStorage::GetItemList(EInventoryTab InTabType)
//{
//	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
//	{
//		ItemType invenTabType = ItemType::MAX;
//		switch (InTabType)
//		{
//		case EInventoryTab::AllTab:
//			invenTabType = ItemType::MAX;
//			break;
//		case EInventoryTab::EquipmentTab:
//			invenTabType = ItemType::EQUIP;
//			break;
//		case EInventoryTab::ConsumableTab:
//			invenTabType = ItemType::CONSUME;
//			break;
//		case EInventoryTab::MaterialTab:
//			invenTabType = ItemType::INGREDIENT;
//			break;
//		}
//
//		_guildOwnedItemCount = 0;
//		TMap<ItemDBId, FGsItem*>& itemList = storageData->GetItemList();
//		for (const auto& itemData : itemList)
//		{
//			if (FGsTimeSyncUtil::GetServerNowSecond() > itemData.Value->GetExpiredTime())
//				continue;
//
//			++_guildOwnedItemCount;
//			if (invenTabType == ItemType::MAX || invenTabType == itemData.Value->GetType())
//			{
//				_dataList.Emplace(itemData.Value);
//			}
//		}
//	}
//	
//	_dataList.Sort([this](FGsItem& A, FGsItem& B)
//		{
//			return (A.GetExpiredTime() > B.GetExpiredTime()) ? true : false;
//		});
//
//	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_DISTRIBUTION, nullptr);
//}
//
//int32 FGsInventoryLogicGuildStorage::GetDataIndex(ItemDBId InItemDBId) const
//{
//	return _dataList.IndexOfByPredicate([InItemDBId](const FGsItem* InItem)
//		{
//			return (nullptr != InItem && InItem->GetDBID() == InItemDBId);
//		}
//	);
//}
//
//FGsItem* FGsInventoryLogicGuildStorage::GetDataByIndex(int32 InIndex)
//{
//	return _dataList.IsValidIndex(InIndex) ? _dataList[InIndex] : nullptr;
//}
//
//void FGsInventoryLogicGuildStorage::OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount)
//{
//	_invenTotalSlotCount = InMaxSlotCount;
//	_totalExtendCount = InTotalExtendCount;
//
//	FText resultText;
//	FText findText;
//	if (true == FText::FindText(TEXT("DepotText"), TEXT("Notice_FullExtend"), findText))
//	{
//		resultText = findText.Format(findText, _totalExtendCount);
//		FGsUIHelper::TrayMessageTicker(resultText);
//	}
//}
//
//ItemAmount FGsInventoryLogicGuildStorage::GetInventorySlotCount() const
//{
//	return _invenTotalSlotCount;
//}
//
//ItemAmount FGsInventoryLogicGuildStorage::GetOwnedItemCount() const
//{
//	return _guildOwnedItemCount;
//}
//
//int32 FGsInventoryLogicGuildStorage::GetEnoughExtendSlotCount() const
//{
//	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
//	{
//		if (storageData->_slotExtendedCount < storageData->ExtendCurrencyList.Num())
//		{
//			return storageData->ExtendCurrencyList.Num() - storageData->_slotExtendedCount;
//		}
//	}
//
//	return 0;
//}
//
//void FGsInventoryLogicGuildStorage::OnCreateItemIcon(UGsUIIconBase* InIcon)
//{
//	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
//	if (nullptr == itemIcon)
//		return;
//
//}
//
//void FGsInventoryLogicGuildStorage::OnClickSlotLock()
//{
//	if (UGsUIPopupCommonPay* popupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay"))))
//	{
//		uint8 enoughCount = GetEnoughExtendSlotCount();
//		if (0 >= enoughCount)
//		{
//			// 이미 다 뚫음
//			return;
//		}
//
//		FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//		if (nullptr == storageData)
//			return;
//
//		FText resultText;
//		FText findText;
//		if (true == FText::FindText(TEXT("DepotText"), TEXT("ExtendSlot_Check"), findText))
//		{
//			resultText = findText.Format(findText, storageData->_slotExtendedCount, storageData->ExtendCurrencyList.Num());
//		}
//
//		if (storageData->ExtendCurrencyList.Contains(storageData->_slotExtendedCount + 1))
//		{
//			TPair<CurrencyType, int32> extendCurrency = storageData->ExtendCurrencyList[storageData->_slotExtendedCount + 1];
//			popupCommonPay->SetData(resultText,
//				FGsCostPackage::CreateSimple(extendCurrency.Key, extendCurrency.Value),
//				[storageData](bool InIsOk)
//				{
//					if (!InIsOk)
//						return;
//
//					if (true == storageData->IsEnoughExtendSlotCost())
//					{
//						FGsNetSendServiceGuild::SendReqGuildStorageExtendSlot();
//						return;
//					}
//
//					// 재화 모자름 ticker 출력
//					FText Text;
//					FText::FindText(TEXT("DepotText"), TEXT("Alert_NoCost"), Text);
//					FGsUIHelper::TrayMessageTicker(Text);
//				});
//		}
//		
//	}
//
//}
//
//void FGsInventoryLogicGuildStorage::UpdateSelectedItem()
//{
//	if (false == _dataList.ContainsByPredicate([this](const FGsItem* itemData)
//		{
//			return itemData->GetDBID() == GetSelectedItemDBId();
//		}))
//	{
//		SetSelectedItemDBId(INVALID_ITEM_DB_ID);
//	}
//}
//
//void FGsInventoryLogicGuildStorage::OnClickItemIcon(UGsUIIconItem& InIcon)
//{
//	FGsItem* itemData = InIcon.GetItemIconData();
//	FGsItem* itemRealData = nullptr;
//	for (uint8 i = 0; i < _dataList.Num(); ++i)
//	{
//		itemRealData = _dataList[i];
//		if (itemData->GetDBID() == itemRealData->GetDBID())
//		{
//			itemRealData->SetIsNew(false);
//		}
//	}
//}
//
//void FGsInventoryLogicGuildStorage::OnSelectTab(EInventoryTab InType)
//{
//	_currTabType = InType;
//}
//
//bool FGsInventoryLogicGuildStorage::CheckRedDotByInvenTab(EInventoryTab InTab)
//{
//	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
//	{
//		ItemType invenTabType = ItemType::MAX;
//		switch (InTab)
//		{
//		case EInventoryTab::AllTab:
//			invenTabType = ItemType::MAX;
//			break;
//		case EInventoryTab::EquipmentTab:
//			invenTabType = ItemType::EQUIP;
//			break;
//		case EInventoryTab::ConsumableTab:
//			invenTabType = ItemType::CONSUME;
//			break;
//		case EInventoryTab::MaterialTab:
//			invenTabType = ItemType::INGREDIENT;
//			break;
//		}
//
//		TMap<ItemDBId, FGsItem*>& itemList = storageData->GetItemList();
//		for (const auto& itemData : itemList)
//		{
//			if (invenTabType == ItemType::MAX || invenTabType == itemData.Value->GetType())
//			{
//				if (false == itemData.Value->IsNew())
//					continue;
//
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//void FGsInventoryLogicGuildStorage::CheckExpiredItem()
//{
//	if (0 >= _dataList.Num())
//		return;
//
//	FGsItem* itemData = _dataList.Last();
//	if (nullptr == itemData)
//		return;
//
//	if (FGsTimeSyncUtil::GetServerNowSecond() > itemData->GetExpiredTime())
//	{
//		GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_INVENTORY, nullptr);
//	}
//}
//
//int32 FGsInventoryLogicGuildStorage::GetDataCount() const
//{
//	ItemAmount ownedCount = _dataList.Num();
//	if (ownedCount > _invenTotalSlotCount)
//	{
//		FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData();
//		if (0 != (ownedCount % storageData->ExtendSlotCount))
//		{
//			uint16 calc1 = ownedCount / storageData->ExtendSlotCount;
//			uint16 result = (calc1 + 1) * storageData->ExtendSlotCount;
//			// 최대 슬롯을 벗어난 임시 아이템을 포함한 슬롯 개수
//			return result;
//		}
//		// 최대 슬롯을 벗어난 임시 아이템을 포함한 슬롯 개수
//		return ownedCount;
//	}
//	// 창고 최대 슬롯 개수
//	return _invenTotalSlotCount;
//}
