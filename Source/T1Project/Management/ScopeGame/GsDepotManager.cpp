#include "GsDepotManager.h"

#include "DataSchema/GsSchemaExtendSlotCost.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/GsSchemaAutoDepotConfig.h"

#include "Item/GsItem.h"
#include "Item/GsItemBuffers.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Currency/GsCurrencyHelper.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Net/GsNetSendServiceItem.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "T1Project/GameObject/Data/GsGameObjectDataCreature.h"
#include "T1Project/Management/ScopeGame/GsGameObjectManager.h"
#include "T1Project/UI/UIContent/Helper/GsUIHelper.h"

void FGsDepotManager::Initialize()
{
	if (nullptr == _depotItemBuffers)
	{
		_depotItemBuffers = new FGsItemBuffers();
	}

	const UGsTableDepotConfig* table = Cast<UGsTableDepotConfig>(FGsSchemaDepotConfig::GetStaticTable());
	if (nullptr == table)
		return;

	TArray<const FGsSchemaDepotConfig*> rowList;
	if (false == table->GetAllRows(rowList))
		return;

	if (0 >= rowList.Num())
		return;
	
	_depotConfigTableData = rowList[0];
	_depotMaxSlotCount = 0;
	_extendedSlotNumber = 0;
}

void FGsDepotManager::Finalize()
{	
	_depotBasketItemInfoList.Empty();
	_autoSettingDataMap.Empty();
	_depotItemBuffers->ClearItems();

	delete _depotItemBuffers;
	_depotItemBuffers = nullptr;

	_depotConfigTableData = nullptr;
}

void FGsDepotManager::ResetData()
{
	_depotBasketItemInfoList.Empty();
	_depotItemBuffers->ResetData();
}

void FGsDepotManager::ResetBasketItems()
{
	_eDepotState = DepotActionState::State_Deposit;
	_depotBasketItemInfoList.Empty();
}

/////////////// Add Item //////////////////
void FGsDepotManager::AddItem(const ItemData* In_addItem)
{
	check(nullptr != In_addItem);

	TWeakPtr<FGsItem> Item = FindDepotItemByDBID(In_addItem->mItemDBId, GetItemTypeByTID(In_addItem->mTemplateId));
	if (true == Item.IsValid() && true == Item.Pin()->IsStackable())
	{
		// 이미 보유중. 덮어씀
		Item.Pin()->UpdateBy(In_addItem);
		Item.Pin()->SetIsDepotItem(true);

		FGsUpdateDepotItem Param(Item.Pin()->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE, &Param);
	}
	else
	{
		// new item
		FGsItem* newItem = FGsItem::Create(In_addItem);
		if (nullptr == newItem)
		{
			GSLOG(Error, TEXT("nullptr == Item"));
			return;
		}
		else
		{
			newItem->SetIsDepotItem(true);
			_depotItemBuffers->AddItems(newItem);

			FGsAddDepotItem Param(newItem->GetDBID());
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_ADD_ITEM, &Param);

			FGsItemHelper::SetPeriodItemCoolTime(newItem);
		}
	}
}

/////////////// Add Item - EquipItem //////////////////
void FGsDepotManager::AddItem(const ItemDataEquip* In_addItem)
{
	check(nullptr != In_addItem);

	TWeakPtr<FGsItem> Item = FindDepotItemByDBID(In_addItem->mItemDBId, GetItemTypeByTID(In_addItem->mTemplateId));
	if (true == Item.IsValid())
	{
		//FGsItem* oldItem = FGsItem::CreateItem(item);
		// 이미 보유중. 덮어씀
		Item.Pin()->UpdateBy(In_addItem);
		Item.Pin()->SetIsDepotItem(true);

		FGsUpdateDepotItem Param(Item.Pin()->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE, &Param);
	}
	else
	{
		// new item
		FGsItem* newItem = FGsItem::Create(In_addItem);
		if (nullptr == newItem)
		{
			GSLOG(Error, TEXT("nullptr == Item"));
			return;
		}
		else
		{
			newItem->SetIsDepotItem(true);
			_depotItemBuffers->AddItems(newItem);

			FGsAddDepotItem Param(newItem->GetDBID());
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_ADD_ITEM, &Param);

			FGsItemHelper::SetPeriodItemCoolTime(newItem);
		}
	}
}

void FGsDepotManager::AddItemList(const TArray<ItemData>& InItemList)
{
	for(const ItemData iter : InItemList)
	{
		if (0 >= iter.mTemplateId)
			continue;

		const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(iter.mTemplateId);
		if (nullptr == commonData)
			continue;

		AddOrUpdateDepotItem(&iter);
	}
}

void FGsDepotManager::AddItemList(const TArray<ItemDataEquip>& InItemList)
{
	for (const ItemData iter : InItemList)
	{
		if (0 >= iter.mTemplateId)
			continue;

		const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(iter.mTemplateId);
		if (nullptr == commonData)
			continue;

		AddOrUpdateDepotItem(&iter);
	}
}

void FGsDepotManager::AddItemList(const TArray<ItemDataCreated>& InItemList)
{
	for (const ItemDataCreated& item : InItemList)
	{
		if (0 >= item.mTemplateId)
		{
			GSLOG(Warning, TEXT("Invalid item.mTemplateId"));
			continue;
		}

		const ItemType itemType = GetItemTypeByTID(item.mTemplateId);
		if (ItemType::EQUIP == itemType)
		{
			const ItemDataEquip equipData(item);
			AddItem(&equipData);
		}
		else
		{
			AddItem(&item);
		}
	}
}

void FGsDepotManager::AddOrUpdateDepotItem(const uint64 InItemDBID, const ItemAmount InAmount)
{
	TWeakPtr<FGsItem> item = FindDepotItemByDBID(InItemDBID);
	if (true == item.IsValid() && true == item.Pin()->IsStackable())
	{
		item.Pin()->SetAmount(InAmount);
		item.Pin()->SetIsDepotItem(true);

		FGsUpdateDepotItem param(item.Pin()->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE, &param);
	}
}

void FGsDepotManager::AddOrUpdateDepotItem(const ItemData* In_addItem)
{
	//GSLOG(Warning, TEXT("FGsDepotManager::AddOrUpdateDepotItem - itemDBID : &llu"), In_addItem->mItemDBId);
	TWeakPtr<FGsItem> item = FindDepotItemByDBID(In_addItem->mItemDBId);
	if (true == item.IsValid() && true == item.Pin()->IsStackable())
	{
		// 이미 보유중. 덮어씀
		item.Pin()->UpdateBy(In_addItem);
		item.Pin()->SetIsDepotItem(true);

		FGsUpdateDepotItem param(item.Pin()->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE, &param);
	}
	else
	{
		// new item
		FGsItem* newItem = FGsItem::Create(In_addItem);
		if (nullptr != newItem)
		{
			newItem->SetIsDepotItem(true);
			_depotItemBuffers->AddItems(newItem);
			FGsAddDepotItem param(newItem->GetDBID());
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_ADD_ITEM, &param);

			FGsItemHelper::SetPeriodItemCoolTime(newItem);
		}
	}
}

void FGsDepotManager::AddOrUpdateDepotItem(const ItemDataEquip* In_addItem)
{
	TWeakPtr<FGsItem> item = FindDepotItemByDBID(In_addItem->mItemDBId);
	if (true == item.IsValid() && true == item.Pin()->IsStackable())
	{
		// 이미 보유중. 덮어씀
		item.Pin()->UpdateBy(In_addItem);
		item.Pin()->SetIsDepotItem(true);

		FGsUpdateDepotItem param(item.Pin()->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE, &param);
	}
	else
	{
		// new item
		FGsItem* newItem = FGsItem::Create(In_addItem);
		if (nullptr != newItem)
		{
			newItem->SetIsDepotItem(true);
			_depotItemBuffers->AddItems(newItem);
			FGsAddDepotItem param(newItem->GetDBID());
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_ADD_ITEM, &param);

			FGsItemHelper::SetPeriodItemCoolTime(newItem);
		}
	}
}

bool FGsDepotManager::AddOrUpdateDepotBasketItem(const uint64 InItemDBID, const ItemAmount InAmount, const DepotActionState InStateType)
{
	if (InStateType != _eDepotState)
	{
		_depotBasketItemInfoList.Empty();

		_eDepotState = InStateType;
		GMessage()->GetItemContents().SendMessage(_eDepotState == DepotActionState::State_Deposit ?
			MessageContentItem::DEPOT_STATE_DEPOSIT : MessageContentItem::DEPOT_STATE_TAKEOUT, nullptr);
	}

	if (0 < _depotBasketItemInfoList.Num())
	{
		if (true == IsFullBasketSlot())
		{
			FText findText;
			FText::FindText(TEXT("DepotText"), TEXT("Alert_FullSlot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

			return false;
		}
	}

	for (TPair<uint64, ItemAmount>& iter : _depotBasketItemInfoList)
	{
		if (iter.Key == InItemDBID)
		{
			iter.Value = InAmount;
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_INVALIDATE, nullptr);
			return true;
		}
	}

	_depotBasketItemInfoList.Emplace(TPair<uint64, ItemAmount>(InItemDBID, InAmount));
	FGsOnChangeDepotBasketItem paramData(InItemDBID);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_ADD, &paramData);

	return true;
}

void FGsDepotManager::UpdateDepotBasketItemList(const TArray<ItemDBId>& InItemDBIdList)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	bool needRefreshUI = false;

	for (ItemDBId updatedItemDBId : InItemDBIdList)
	{
		// TPair<ItemDBId, ItemAmount>
		for (TPair<uint64, ItemAmount>& basketItemData : _depotBasketItemInfoList)
		{
			if (basketItemData.Key == updatedItemDBId)
			{
				TWeakPtr<FGsItem> itemData = itemManager->FindItem(updatedItemDBId);
				if (itemData.IsValid())
				{
					basketItemData.Value = FMath::Min(basketItemData.Value, itemData.Pin().Get()->GetAmount());
					needRefreshUI = true;
				}
				break;
			}
		}
	}

	if (needRefreshUI)
	{
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_INVALIDATE, nullptr);
	}
}

void FGsDepotManager::RemoveDepotItem(const uint64 InItemDBID)
{
	TWeakPtr<FGsItem> findItem = FindDepotItemByDBID(InItemDBID);
	if (nullptr != findItem)
	{
		int32 itemTID = findItem.Pin()->GetTID();
		_depotItemBuffers->RemoveItem(findItem.Pin().Get());
		
		FGsRemoveDepotItem paramData(itemTID, InItemDBID);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_REMOVE_ITEM, &paramData);
	}
}

void FGsDepotManager::RemoveDepotItemList(const TArray<ItemDBId>& InItemDBIdList)
{
	for (ItemDBId itemDBId : InItemDBIdList)
	{
		RemoveDepotItem(itemDBId);
	}
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE_ALL, nullptr);
}

void FGsDepotManager::RemoveDepotBasketItem(const uint64 InItemDBID)
{
	bool isFound = false;
	int32 removeIndex = 0;
	for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
	{
		TPair<uint64, ItemAmount>& data = _depotBasketItemInfoList[i];
		if (data.Key == InItemDBID)
		{
			removeIndex = i;

			isFound = true;
			break;
		}
	}

	if (true == isFound)
	{
		_depotBasketItemInfoList.RemoveAt(removeIndex);

		FGsOnChangeDepotBasketItem paramData(InItemDBID);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_REMOVE, &paramData);
	}
}

bool FGsDepotManager::GetBasketItemInfoBySlotPageIndex(uint8 InStartIndexNum, uint8 InSlotIndex, OUT uint64& outDBID, OUT ItemAmount& outAmount)
{
	if (0 >= _depotBasketItemInfoList.Num())
		return false;

	int32 slotIndex = 0;
	for (int32 i = InStartIndexNum; i < _depotBasketItemInfoList.Num(); ++i)
	{
		if (false == _depotBasketItemInfoList.IsValidIndex(i))
			return false;

		if (InSlotIndex == slotIndex)
		{
			TPair<uint64, ItemAmount>& pairData = _depotBasketItemInfoList[i];
			outDBID = pairData.Key;
			outAmount = pairData.Value;
			/*
#if WITH_EDITOR
			GSLOG(Warning, TEXT("GetBasketItemInfoBySlotPageIndex - pairData.Key : %llu"), pairData.Key);
			GSLOG(Warning, TEXT("GetBasketItemInfoBySlotPageIndex - pairData.Value : %d"), pairData.Value);

			GSLOG(Warning, TEXT("GetBasketItemInfoBySlotPageIndex - outDBID : %llu"), outDBID);
			GSLOG(Warning, TEXT("GetBasketItemInfoBySlotPageIndex - outAmount : %d"), outAmount);
#endif
*/
			return true;
		}
		++slotIndex;
	}

	return false;
}

TWeakPtr<FGsItem> FGsDepotManager::FindDepotItemByDBID(const uint64 In_ItemDBID, const ItemType In_ItemType)
{
	return _depotItemBuffers->Find(In_ItemDBID, In_ItemType);
}

void FGsDepotManager::GetItemDBIDByStorageType(EInventoryTab InTabType, uint8 InSortingTypeIndex ,OUT TArray<uint64>& outDBIDList)
{
	outDBIDList.Empty();
	TArray<ItemType> typeList;
	switch (InTabType)
	{
	case EInventoryTab::AllTab:
		typeList.Emplace(ItemType::EQUIP);
		typeList.Emplace(ItemType::CONSUME);
		typeList.Emplace(ItemType::INGREDIENT);
		typeList.Emplace(ItemType::CURRENCY);
		break;
	case EInventoryTab::EquipmentTab:
		typeList.Emplace(ItemType::EQUIP);
		break;
	case EInventoryTab::ConsumableTab:
		typeList.Emplace(ItemType::CONSUME);
		break;
	case EInventoryTab::MaterialTab:
		typeList.Emplace(ItemType::INGREDIENT);
		typeList.Emplace(ItemType::CURRENCY);
		break;
	}

	for (uint8 i = 0; i < typeList.Num(); ++i)
	{
		TArray<FGsItem*> dataList;
		TArray<TWeakPtr<FGsItem>> storageItems = _depotItemBuffers->FindByItemType(typeList[i]);
		for (int32 j = 0; j < storageItems.Num(); ++j)
		{
			if (false == storageItems[j].IsValid())
				continue;

			outDBIDList.Emplace(storageItems[j].Pin()->GetDBID());
		}
	}

	SortItemListByDBID(InSortingTypeIndex,outDBIDList);
}

TArray<TWeakPtr<FGsItem>> FGsDepotManager::FindByTID(const ItemId In_ItemTID, const ItemType In_ItemType)
{
	TArray<TWeakPtr<FGsItem>> ownItemList = _depotItemBuffers->FindByTID(In_ItemTID, (ItemType::MAX == In_ItemType) ?
		GetItemTypeByTID(In_ItemTID) : In_ItemType);

	return ownItemList;
}

void FGsDepotManager::SaveAutoDepositSettingData(TMap<ItemId, bool>& InSettingDataMap, const TArray<TPair<int32, ItemGrade>>& InGroupGradeMap)
{
	bool isNeedSave = false;
	for (const TPair<ItemId, bool> settingData : InSettingDataMap)
	{
		bool prevData = false;
		if (_autoSettingDataMap.Contains(settingData.Key))
		{
			prevData = _autoSettingDataMap[settingData.Key];
		}

		if (prevData != settingData.Value)
		{
			isNeedSave = true;
			break;
		}
	}

	for (const TPair<int32, ItemGrade>& groupGradeData : InGroupGradeMap)
	{
		ItemGrade prevGrade = GetAutoDepositGrade(groupGradeData.Key);
		if (prevGrade != groupGradeData.Value)
		{
			isNeedSave = true;
			break;
		}
	}

	if (false == isNeedSave)
		return;

	TArray<ItemId> saveItemIdList;
	for (const TPair<ItemId, bool> settingData : InSettingDataMap)
	{
		if (true == settingData.Value)
		{
			saveItemIdList.Emplace(settingData.Key);
		}
	}

	TArray<AutoDepotConfigPair> saveGroupGradeList;
	for (const TPair<int32, ItemGrade>& groupGradeData : InGroupGradeMap)
	{
		AutoDepotConfigPair pairData(groupGradeData.Key, groupGradeData.Value);
		saveGroupGradeList.Emplace(pairData);
	}

	FGsNetSendServiceItem::SendOnChangeAutoDeposit(saveItemIdList);
	FGsNetSendServiceItem::SendOnChangeAutoDepositGrade(saveGroupGradeList);
}

ItemGrade FGsDepotManager::GetAutoDepositGrade(int32 InGroupId)
{
	if (_autoDepositGradeMap.Contains(InGroupId))
	{
		return _autoDepositGradeMap[InGroupId];
	}

	return ItemGrade::NONE;
}

bool FGsDepotManager::IsSelectedAutoSlot(const uint32 InItemTID)
{
	if (0 >= _autoSettingDataMap.Num())
		return false;

	if (false == _autoSettingDataMap.Contains(InItemTID))
		return false;

	return _autoSettingDataMap[InItemTID];
}

bool FGsDepotManager::IsLowerOrEqualAutoDepositGrade(int32 InGroupId, ItemGrade InGrade)
{
	if (_autoDepositGradeMap.Contains(InGroupId))
	{
		int32 inGradeValue = static_cast<int32>(InGrade);
		int32 savedGradeValue = static_cast<int32>(_autoDepositGradeMap[InGroupId]);
		return savedGradeValue >= inGradeValue;
	}
	return false;
}

void FGsDepotManager::AutoDepositProc()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	const UGsTableAutoDepotConfig* autoDepotConfigTable = Cast<UGsTableAutoDepotConfig>(FGsSchemaAutoDepotConfig::GetStaticTable());
	if (nullptr == autoDepotConfigTable)
		return;

	TArray<const FGsSchemaAutoDepotConfig*> autoDepotConfigList;
	if (false == autoDepotConfigTable->GetAllRows(autoDepotConfigList))
		return;

	for (const FGsSchemaAutoDepotConfig* autoDepotConfig : autoDepotConfigList)
	{
		if (_autoDepositGradeMap.Contains(autoDepotConfig->groupId))
		{
			for (const FGsSchemaItemCommonRow& itemCommonRow : autoDepotConfig->itemIdList)
			{
				const FGsSchemaItemCommon* itemCommon = itemCommonRow.GetRow();
				if (nullptr == itemCommon)
					continue;

				int32 itemGradeValue = static_cast<int32>(itemCommon->grade);
				int32 savedGradeValue = static_cast<int32>(_autoDepositGradeMap[autoDepotConfig->groupId]);
				if (_autoSettingDataMap.Contains(itemCommon->id))
				{
					_autoSettingDataMap[itemCommon->id] |= savedGradeValue >= itemGradeValue;
				}
				else
				{
					_autoSettingDataMap.Emplace(itemCommon->id, savedGradeValue >= itemGradeValue);
				}
			}
		}
	}

	// 자동 보관 설정된 정보가 존재하는가?
	if (0 >= _autoSettingDataMap.Num())
	{
		FText findText;
		FText::FindText(TEXT("DepotText"), TEXT("Empty_AutoDepositSetting"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	TArray<uint64> addedItemDBIdList;
	TArray<TWeakPtr<FGsItem>> inventoryItemList = itemManager->FindAllItems();
	for (TWeakPtr<FGsItem> itemData : inventoryItemList)
	{
		if (false == itemData.IsValid())
			continue;

		// 자동 보관할 수 있는 물품인가?
		if (false == IsSelectedAutoSlot(itemData.Pin()->GetTID()))
			continue;

		// 창고 상태 변경 (찾기 -> 보관)
		ChangeDepotState(DepotActionState::State_Deposit);

		// 아이템 정보가 갱신되는 상황인지 확인
		bool isUpdated = false;
		for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
		{
			TPair<uint64, ItemAmount>& data = _depotBasketItemInfoList[i];
			if (data.Key == itemData.Pin()->GetDBID())
			{
				data.Value = itemData.Pin()->GetAmount();
				isUpdated = true;
				break;
			}
		}

		if (isUpdated)
		{
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_INVALIDATE, nullptr);
			continue;
		}

		// 아이템이 추가되는 상황
		// 목록이 가득 찼는가?
		if (IsFullBasketSlot())
		{
			FText findText;
			FText::FindText(TEXT("DepotText"), TEXT("Alert_FullSlot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			continue;
		}

		_depotBasketItemInfoList.Emplace(TPair<uint64, ItemAmount>(itemData.Pin()->GetDBID(), itemData.Pin()->GetAmount()));
		addedItemDBIdList.Emplace(itemData.Pin()->GetDBID());
	}

	if (addedItemDBIdList.Num() > 0)
	{
		FGsOnAddDepotBasketItemList paramData(addedItemDBIdList);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_ADD_LIST, &paramData);
	}
}

const TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>& FGsDepotManager::GetItemBuffers() const
{
	check(_depotItemBuffers);
	return _depotItemBuffers->GetAllItems();
}

bool FGsDepotManager::IsNeedCalculator(const uint64 InItemDBID , bool InIsFromInventory)
{
	TWeakPtr<FGsItem> itemData = true == InIsFromInventory ? GItem()->FindItem (InItemDBID) : FindDepotItemByDBID(InItemDBID);
	if (false == itemData.IsValid())
		return false;

	return 1 < itemData.Pin()->GetAmount();
}

void FGsDepotManager::SendDepositItems()
{
	if (0 >= _depotBasketItemInfoList.Num())
		return;

	int32 expectCost = GetBasketItemsPrice();
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, expectCost, false))
	{
		// 골드 부족 시엔 팝업으로 띄움
		Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, expectCost - myAmount);
		return;
	}

	//if (true == IsFullDepotSlot())
	//{
	//	FText findText;
	//	FText::FindText(TEXT("DepotText"), TEXT("Alert_FullDepot"), findText);
	//	FGsUIHelper::TrayMessageTicker(findText);

	//	return;
	//}

	TArray<ItemDBIdAmountPair> itemPairDataList;
	for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
	{
		TPair<uint64,int32> pairData = _depotBasketItemInfoList[i];
		if(0 >= pairData.Key || 0 >= pairData.Value)
			continue;

		ItemDBIdAmountPair packetData(pairData.Key, pairData.Value);
		itemPairDataList.Emplace(packetData);
	}

	FGsNetSendServiceItem::SendDepositItems(_npcCreatureID, itemPairDataList);
}

void FGsDepotManager::AckDepotPacket()
{
	_depotBasketItemInfoList.Empty();

	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
		return;

	// 중간 영역 갱신 : 물품 목록, 비용, 무게
	msgHolder->GetItemContents().SendMessage(MessageContentItem::DEPOT_BASKET_INVALIDATE, nullptr);

	// 우측 인벤 영역 갱신
	msgHolder->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVEN_INVALIDATE, nullptr);

	// 좌측 창고 영역 갱신
	msgHolder->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE_ALL, nullptr);
}

void FGsDepotManager::SendTakeItems()
{
	if (0 >= _depotBasketItemInfoList.Num())
		return;

	int32 expectCost = GetBasketItemsPrice();
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, expectCost, false))
	{
		// 골드 부족 시엔 팝업으로 띄움
		Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, expectCost - myAmount);
		return;
	}

	if (true == IsFullExpectWeight())
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return;
	}

	//if (true == IsFullInvenSlot())
	//{
	//	FText findText;
	//	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
	//	FGsUIHelper::TrayMessageTicker(findText);

	//	return;
	//}

	TArray<ItemDBIdAmountPair> itemPairDataList;
	for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
	{
		TPair<uint64, int32> pairData = _depotBasketItemInfoList[i];
		if (0 >= pairData.Key || 0 >= pairData.Value)
			continue;

		ItemDBIdAmountPair packetData(pairData.Key, pairData.Value);
		itemPairDataList.Emplace(packetData);
	}

	FGsNetSendServiceItem::SendTakeItems(_npcCreatureID, itemPairDataList);
}

int32 FGsDepotManager::GetBasketItemsPrice()
{
	// 보관 or 찾기의 총 금액 반환
	int32 price = DepotActionState::State_Deposit == _eDepotState ? _depotConfigTableData->depositCost : _depotConfigTableData->withdrawCost;
	int32 result = _depotBasketItemInfoList.Num() * price;
	return result;
}

int32 FGsDepotManager::GetExpectWeight()
{
	int32 ownedItemTotalWeight = GItem()->GetOwnedItemWeight();
	int32 basketTotalWeight = GetBasketTotalWeight();

	// 찾을때는 무게를 더하지만 맡길때는 무게를 뺀다.
	int32 finalWeight = (DepotActionState::State_Deposit == _eDepotState) ?
		(ownedItemTotalWeight - basketTotalWeight) : (ownedItemTotalWeight + basketTotalWeight);

	return finalWeight;
}

int32 FGsDepotManager::GetBasketTotalWeight()
{
	UGsItemManager* itemMgr = GItem();
	int32 totalWeight = 0;
	for (const auto& iter : _depotBasketItemInfoList)
	{
		TWeakPtr<FGsItem> itemData = DepotActionState::State_Deposit == _eDepotState ? 
			itemMgr->FindItem(iter.Key) : FindDepotItemByDBID(iter.Key);

		if(false == itemData.IsValid())
			continue;
		
		totalWeight += (itemData.Pin()->GetWeight() * iter.Value);
	}

	return totalWeight;
}

bool FGsDepotManager::IsFullBasketSlot()
{
	return BASKET_MAX_SLOT_COUNT <= _depotBasketItemInfoList.Num();
}

bool FGsDepotManager::IsFullExpectWeight()
{
	if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
	{
		if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
		{
			return statInfo->GetStatValue() <= GetExpectWeight();
		}
	}

	return false;
}

bool FGsDepotManager::IsFullDepotSlot()
{
	UGsItemManager* itemMgr = GItem();
	uint16 owendCnt = _depotItemBuffers->GetOwnedItemCount();
	if ((owendCnt + _depotBasketItemInfoList.Num()) > _depotMaxSlotCount)
	{
		int32 expectRealOverCount = 0;
		// over 된 상황
		for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
		{
			TPair<uint64, int32> pairData = _depotBasketItemInfoList[i];
			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(pairData.Key);
			if(false == findItem.IsValid())
				continue;

			if (false == findItem.Pin()->IsStackable())
			{
				// 새로운 슬롯이 필요 한 상태
				++expectRealOverCount;
				continue;
			}
			else
			{
				TArray<TWeakPtr<FGsItem>> findItems = _depotItemBuffers->FindByTID(findItem.Pin()->GetTID(), findItem.Pin()->GetType());
				if (0 >= findItems.Num())
				{
					// 새로운 슬롯이 필요 한 상태
					++expectRealOverCount;
					continue;
				}
				else
				{
					bool enoughCount = false;
					for (int32 j = 0; j < findItems.Num(); ++j)
					{
						if(false == findItems[j].IsValid())
							continue;

						ItemAmount amount = findItems[j].Pin()->GetAmount() + pairData.Value;
						if (findItems[j].Pin()->GetMaxStackCount() >= amount)
						{
							enoughCount = true;
							break;
						}
					}
					if (false == enoughCount)
					{
						// 새로운 슬롯이 필요 한 상태
						++expectRealOverCount;
					}
				}
			}
		}
		if ((owendCnt + expectRealOverCount) > _depotMaxSlotCount)
		{
			return true;
		}
	}

	return false;
}

bool FGsDepotManager::IsFullInvenSlot()
{
	UGsItemManager* itemMgr = GItem();
	uint16 maxCount = GSInventory()->GetInvenSlotMaxCount();
	int32 owendCnt = GItem()->GetOwnedItemCount();
	if ((owendCnt + _depotBasketItemInfoList.Num()) > maxCount)
	{
		int32 expectRealOverCount = 0;
		// over 된 상황
		for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
		{
			TPair<uint64, int32> pairData = _depotBasketItemInfoList[i];
			TWeakPtr<FGsItem> findItem = FindDepotItemByDBID(pairData.Key);
			if (false == findItem.IsValid())
				continue;

			if (false == findItem.Pin()->IsStackable())
			{
				// 새로운 슬롯이 필요 한 상태
				++expectRealOverCount;
				continue;
			}
			else
			{
				TArray<TWeakPtr<FGsItem>> findItems = itemMgr->FindByTID(findItem.Pin()->GetTID(), findItem.Pin()->GetType());
				if (0 >= findItems.Num())
				{
					// 새로운 슬롯이 필요 한 상태
					++expectRealOverCount;
					continue;
				}
				else
				{
					bool enoughCount = false;
					for (int32 j = 0; j < findItems.Num(); ++j)
					{
						if (false == findItems[j].IsValid())
							continue;

						ItemAmount amount = findItems[j].Pin()->GetAmount() + pairData.Value;
						if (findItems[j].Pin()->GetMaxStackCount() >= amount)
						{
							enoughCount = true;
							break;
						}
					}
					if (false == enoughCount)
					{
						// 새로운 슬롯이 필요 한 상태
						++expectRealOverCount;
					}
				}
			}
		}
		if ((owendCnt + expectRealOverCount) > maxCount)
		{
			return true;
		}
	}

	return false;


}

// InSortTypeIndex - 0 : default , 1 : grade , 2 : weight
void FGsDepotManager::SortItemListByDBID(uint8 InSortTypeIndex, OUT TArray<uint64>& outDBIDList)
{
	TArray<FGsItem*> itemList;
	itemList.Empty();
	for (int32 i = 0; i < outDBIDList.Num(); ++i)
	{
		TWeakPtr<FGsItem> item = FindDepotItemByDBID(outDBIDList[i]);
		if (false == item.IsValid())
			continue;

		itemList.Emplace(item.Pin().Get());
	}

	switch (InSortTypeIndex)
	{
	case 0:
		GItem()->SortDefaultType(itemList);
		break;
	case 1:
		GItem()->SortGradeType(itemList);
		break;
	case 2:
		GItem()->SortWeightType(itemList);
		break;
	}

	outDBIDList.Empty();
	for (int32 i = 0; i < itemList.Num(); ++i)
	{
		outDBIDList.Emplace(itemList[i]->GetDBID());
	}
}

bool FGsDepotManager::GetNextExtendSlotCurrenyData(OUT CurrencyPair& outPairData)
{
	if (0 >= _depotConfigTableData->extendList.Num())
		return false;

	int32 defaultSlotLine =  _depotConfigTableData->defaultSlotCount / _depotConfigTableData->extendSlotCount;
	int32 maxSlotLine = defaultSlotLine + _depotConfigTableData->extendList.Num();
	if (maxSlotLine <= _extendedSlotNumber)
		return false;

	int32 nextNumber = _extendedSlotNumber + 1;
	for (int32 i = 0; i < _depotConfigTableData->extendList.Num(); ++i)
	{
		if(nextNumber != _depotConfigTableData->extendList[i].extendNumber)
			continue;

		outPairData.mCurrencyType = _depotConfigTableData->extendList[i].extendSlotCurrencyType;
		outPairData.mCurrency = _depotConfigTableData->extendList[i].extendSlotCurrencyCost;

		return true;
	}
	return false;
}

void FGsDepotManager::GetExtendSlotCurrenyData(OUT CurrencyPair& outPairData)
{
	if (0 >= _depotConfigTableData->extendList.Num())
		return;

	for (int32 i = 0; i < _depotConfigTableData->extendList.Num(); ++i)
	{
		if (_extendedSlotNumber + 1 != _depotConfigTableData->extendList[i].extendNumber)
			continue;

		outPairData.mCurrencyType = _depotConfigTableData->extendList[i].extendSlotCurrencyType;
		outPairData.mCurrency = _depotConfigTableData->extendList[i].extendSlotCurrencyCost;

		break;
	}
}

Currency FGsDepotManager::GetExtendSlotCurrency(int InSlotLevel)
{
	if (_depotConfigTableData->extendList.Num() < InSlotLevel)
	{
		GSLOG(Error, TEXT("_depotConfigTableData->extendList.Num() < InSlotLevel"));
		return 0;
	}

	InSlotLevel = FMath::Clamp(InSlotLevel, 1, _depotConfigTableData->extendList.Num());

	return _depotConfigTableData->extendList[InSlotLevel-1].extendSlotCurrencyCost;
}

uint16 FGsDepotManager::GetEnoughDepotExtendLineCount() const
{
	return _depotConfigTableData->extendList.Num() - _extendedSlotNumber;
}

bool FGsDepotManager::IsEnoughExtendDepotSlotCost()
{
	CurrencyPair pairData;
	if (true == GetNextExtendSlotCurrenyData(pairData))
	{
		uint64 curValue = FGsCurrencyHelper::GetCurrencyAmount(pairData.mCurrencyType);
		if (curValue >= pairData.mCurrency)
		{
			return true;
		}
	}

	return false;
}

bool FGsDepotManager::CanExtendDepotSlot()
{
	CurrencyPair pairData;
	return GetNextExtendSlotCurrenyData(pairData);
}

void FGsDepotManager::SendExtendDepotSlot(uint8 InExtendLevel)
{
	FGsNetSendServiceItem::SendExtendDepotSlot(InExtendLevel);
}

int32 FGsDepotManager::GetDepotWithdrawCost() const
{
	if (nullptr == _depotConfigTableData)
		return 0;

	return _depotConfigTableData->withdrawCost;
}

int32 FGsDepotManager::GetBasketItemCount() const
{
	return _depotBasketItemInfoList.Num();
}

bool FGsDepotManager::IsExistItemInBasketByDBID(const uint64 InItemDBID)
{
	if (0 >= _depotBasketItemInfoList.Num())
		return false;

	for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
	{
		TPair<uint64,int32> pairData = _depotBasketItemInfoList[i];
		if (InItemDBID != pairData.Key)
			continue;
		
		return true;
	}

	return false;
}

bool FGsDepotManager::IsAllowedDepositItem(const uint64 InItemDBID)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDBID);
	if (false == findItem.IsValid())
		return false;

	return ((false == findItem.Pin()->IsEquipped()) && (true == findItem.Pin()->IsWarehouseStorable()) && (false == findItem.Pin()->IsPresetEquipped()));
}

void FGsDepotManager::OnChangeAutoDepositSettingData(TArray<ItemId>& InItemIdList)
{
	_autoSettingDataMap.Empty();
	for (const ItemId id : InItemIdList)
	{
		if (false == _autoSettingDataMap.Contains(id))
		{
			_autoSettingDataMap.Emplace(id,true);
		}
	}
}

void FGsDepotManager::OnChangeAutoDepositGradeData(TArray<AutoDepotConfigPair>& InAutoDepotConfigList)
{
	_autoDepositGradeMap.Empty();
	for (const AutoDepotConfigPair& autoDepotConfigData : InAutoDepotConfigList)
	{
		if (false == _autoDepositGradeMap.Contains(autoDepotConfigData.mAutoDepotGroupId))
		{
			_autoDepositGradeMap.Emplace(autoDepotConfigData.mAutoDepotGroupId, autoDepotConfigData.mItemGrade);
		}
	}

	const UGsTableAutoDepotConfig* autoDepotConfigTable = Cast<UGsTableAutoDepotConfig>(FGsSchemaAutoDepotConfig::GetStaticTable());
	if (nullptr == autoDepotConfigTable)
		return;

	TArray<const FGsSchemaAutoDepotConfig*> autoDepotConfigList;
	if (false == autoDepotConfigTable->GetAllRows(autoDepotConfigList))
		return;

	for (const FGsSchemaAutoDepotConfig* autoDepotConfig : autoDepotConfigList)
	{
		if (false == _autoDepositGradeMap.Contains(autoDepotConfig->groupId))
		{
			_autoDepositGradeMap.Emplace(autoDepotConfig->groupId, ItemGrade::NONE);
		}
	}
}

ItemType FGsDepotManager::GetItemTypeByTID(const int32 In_ItemTID)
{
	const FGsSchemaItemCommon* itemData = UGsItemManager::GetItemTableDataByTID(In_ItemTID);
	if (nullptr != itemData)
	{
		return itemData->type;
	}
	return ItemType::NONE;
	
}

FGsItem* FGsDepotManager::GetGsItemLowPointer(TWeakPtr<FGsItem> InWeakPtr)
{
	if (false == InWeakPtr.IsValid() || nullptr == InWeakPtr.Pin().Get())
		return nullptr;

	return InWeakPtr.Pin().Get();
}

bool FGsDepotManager::GetItemIndexInBasketByDBID(const uint64 InItemDBID, OUT int32& outIndex)
{
	if (0 >= _depotBasketItemInfoList.Num())
		return false;

	for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
	{
		TPair<uint64, int32> pairData = _depotBasketItemInfoList[i];
		if (InItemDBID == pairData.Key)
		{
			outIndex = i;
			return true;
		}
	}

	return false;
}

int32 FGsDepotManager::GetItemTotalWeightInBasket(const ItemDBId InItemDBID)
{
	if (0 >= _depotBasketItemInfoList.Num())
		return 0.f;

	for (int32 i = 0; i < _depotBasketItemInfoList.Num(); ++i)
	{
		TPair<uint64, int32> pairData = _depotBasketItemInfoList[i];
		if (InItemDBID == pairData.Key)
		{
			TWeakPtr<FGsItem> itemData = (DepotActionState::State_Deposit == _eDepotState) ?
				GItem()->FindItem(InItemDBID) : FindDepotItemByDBID(InItemDBID);

			return itemData.Pin()->GetWeight() * pairData.Value;
		}
	}

	return 0.f;
}

uint16 FGsDepotManager::GetOwnedItemSlotCount() const
{
	return _depotItemBuffers->GetOwnedItemCount();
}

void FGsDepotManager::ChangeDepotState(DepotActionState InState)
{
	if (InState == _eDepotState)
		return;

	_depotBasketItemInfoList.Empty();

	_eDepotState = InState;
	GMessage()->GetItemContents().SendMessage(_eDepotState == DepotActionState::State_Deposit ?
		MessageContentItem::DEPOT_STATE_DEPOSIT : MessageContentItem::DEPOT_STATE_TAKEOUT, nullptr);
}