// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInventoryManager.h"

#include "Item/GsItem.h"
#include "Item/GsItemHelper.h"
#include "Item/GsItemManager.h"
#include "Net/GsNetSendServiceItem.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"

#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupInventory.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "Currency/GsCurrencyHelper.h"
#include "Option/GsGameUserSettings.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "T1Project.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"



void UGsInventoryManager::Initialize()
{
	LoadConfigTable();
	RegisterMsg();

	_itemMgr = GItem();
}

void UGsInventoryManager::Finalize()
{
	RemoveMsg();
	_invenItemDbidMap.Empty();

	_itemMgr.Reset();
	_restoreItemDBId = INVALID_ITEM_DB_ID;
}

void UGsInventoryManager::RegisterMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	// inven
	_listInvenDelegate.Emplace(msg->GetInventoryEvent().AddUObject(MessageContentInven::OPEN,
		this, &UGsInventoryManager::OpenInventoryTab));
	_listInvenDelegate.Emplace(msg->GetInventoryEvent().AddUObject(MessageContentInven::UPDATE_INVEN_SLOT_INDEX,
		this, &UGsInventoryManager::OnChangeInvenIndex));
	
	// item
	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::ADDITEMLIST,
		this, &UGsInventoryManager::AddItemList));

	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEMLIST,
		this, &UGsInventoryManager::UpdateItemList));

	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEMS,
		this, &UGsInventoryManager::RemoveItemList));

	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::LOCKITEM,
		this, &UGsInventoryManager::OnChangeIconDimmed));

	// gameobject
	_listGameObjectDelegate.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::PLAYER_STAT_UPDATE,
		this, &UGsInventoryManager::OnPlayerStatUpdate));
	_listGameObjectDelegate.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_PRESET_EQUIP,
		this, &UGsInventoryManager::OnUserPresetEquipUpdate));

	// hud
	_contentsDelegate = msg->GetContents().AddUObject(MessageContents::ACTIVE_INVENTORY,
		this, &UGsInventoryManager::OpenInventoryByHUD);
}

void UGsInventoryManager::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;
	
	for (TPair<MessageItem, FDelegateHandle>& elemItem : _listItemDelegate)
	{
		msg->GetItem().Remove(elemItem);
	}
	_listItemDelegate.Empty();

	for (TPair<MessageContentInven, FDelegateHandle>& elemInven : _listInvenDelegate)
	{
		msg->GetInventoryEvent().Remove(elemInven);
	}
	_listInvenDelegate.Empty();

	for (MsgGameObjHandle& elemGameObj : _listGameObjectDelegate)
	{
		msg->GetGameObject().Remove(elemGameObj);
	}
	_listGameObjectDelegate.Empty();
	
	msg->GetContents().Remove(_contentsDelegate);
}

void UGsInventoryManager::ResetItemList()
{
	_invenItemDbidMap.Empty();
}

void UGsInventoryManager::OpenInventoryByHUD()
{
	OpenInventory(EInventoryTab::AllTab);
}

void UGsInventoryManager::OnUserPresetEquipUpdate(const struct IGsMessageParam* inParam)
{
	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->InvalidateItemList();
	}
}

void UGsInventoryManager::OpenInventoryTab(const struct IGsMessageParam* inParam)
{
	if (nullptr == inParam)
		return;

	const FGsOpenInventory* param = inParam->Cast<const FGsOpenInventory>();
	if (nullptr == param)
		return;

	OpenInventory(param->_tabType);
}

void UGsInventoryManager::OpenInventory(EInventoryTab InTabType)
{
	/*
	UGsItemManager* itemMgr = GItem();
	TMap<ItemDBId, bool> displayItemMap;
	bool isReaction = itemMgr->GetReactionLockFlag();
	for (FGsItem* iter : InItems)
	{
		if (nullptr == iter)
			continue;

		bool state = (isReaction) && (itemMgr->IsReactionLockTypeItem(iter->GetSubCategory()));
		displayItemMap.Emplace(iter->GetDBID(), state);
	}

	FGsInvenItemList param(displayItemMap, isReaction);
	GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::SendOwnedItemList, param);
	*/

	// Inventory UI Open
	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupInventory")))
		{
			uiManager->CloseByKeyName(TEXT("PopupInventory"));
			return;
		}
		TWeakObjectPtr<UGsUIWidgetBase> widgetBase = uiManager->OpenAndGetWidget("PopupInventory");
		if (widgetBase.IsValid())
		{
			UGsUIPopupInventory* invenUI = Cast<UGsUIPopupInventory>(widgetBase);
			_invenUIPtr = invenUI;

			if (false == _itemMgr.IsValid())
			{
				_itemMgr = GItem();
			}
			invenUI->CreateItemList(GetDisplayItemList(InTabType), _itemMgr->GetReactionLockFlag());
			invenUI->OnChangeTab(InTabType, false);
		}
	}
}

void UGsInventoryManager::CreateItemList()
{
	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}

	if (0 < _invenItemDbidMap.Num())
	{
		_invenItemDbidMap.Empty();
	}

	TArray<TWeakPtr<FGsItem>> findAllItems = _itemMgr->FindAllItems();
	for (TWeakPtr<FGsItem> iter : findAllItems)
	{
		EInventoryTab invenTabType = ConvertTabType(iter.Pin()->GetType());
		if (_invenItemDbidMap.Contains(invenTabType))
		{
			TArray<TPair<ItemType,ItemDBId>>& itemPairDbidList =  _invenItemDbidMap[invenTabType];
			itemPairDbidList.Emplace(TPair<ItemType, ItemDBId>(iter.Pin()->GetType(), iter.Pin()->GetDBID()));
		}
		else
		{
			TArray<TPair<ItemType, ItemDBId>> pairList;
			pairList.Emplace(TPair<ItemType, ItemDBId>(iter.Pin()->GetType(), iter.Pin()->GetDBID()));
			_invenItemDbidMap.Emplace(invenTabType, pairList);
		}
	}
}

EInventoryTab UGsInventoryManager::ConvertTabType(const ItemType InItemType)
{
	switch (InItemType)
	{
	case ItemType::EQUIP:
		return EInventoryTab::EquipmentTab;
	case ItemType::CONSUME:
		return EInventoryTab::ConsumableTab;
	case ItemType::INGREDIENT:
	case ItemType::CURRENCY:
		return EInventoryTab::MaterialTab;

	default:
		return EInventoryTab::MaterialTab;
	}
}

void UGsInventoryManager::AddItemList(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* paramData = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	if (0 >= paramData->_itemDbidList.Num())
	{
		return;
	}

	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}

	TArray<TWeakPtr<FGsItem>> addItemList;
	for (ItemDBId iter : paramData->_itemDbidList)
	{
		if(0 >= iter)
			continue;

		TWeakPtr<FGsItem> findItem = _itemMgr->FindItem(iter);
		if (false == findItem.IsValid())
			return;

		EInventoryTab tab = ConvertTabType(findItem.Pin()->GetType());
		if (_invenItemDbidMap.Contains(tab))
		{
			TArray<TPair<ItemType, ItemDBId>>& itemPairDbidList = _invenItemDbidMap[tab];
			itemPairDbidList.Emplace(TPair<ItemType, ItemDBId>(findItem.Pin()->GetType(), findItem.Pin()->GetDBID()));
		}
		else
		{
			TArray<TPair<ItemType, ItemDBId>> pairList;
			pairList.Emplace(TPair<ItemType, ItemDBId>(findItem.Pin()->GetType(), findItem.Pin()->GetDBID()));
			_invenItemDbidMap.Emplace(tab, pairList);
		}
		addItemList.Emplace(findItem);
	}

	UpdateItemWeightRate();
	CheckItemSlotCount();

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->AddItemList(addItemList);
	}
}

void UGsInventoryManager::CheckItemSlotCount()
{
	if (0 >= _inventorySlotCount)
		return;

	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}
	if (_itemMgr->GetOwnedItemCount() > _inventorySlotCount)
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

void UGsInventoryManager::UpdateItemList(struct FGsItemMessageParamBase& InParam)
{
	FGsUpdateItemList* paramData = static_cast<FGsUpdateItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	UpdateItemWeightRate();

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		if (ItemStorageType::INVENTORY == paramData->_itemStorageType)
		{
			_invenUIPtr->UpdateItemList(paramData->_itemDbidList);
		}
	}
}

void UGsInventoryManager::RemoveItemList(struct FGsItemMessageParamBase& InParam)
{
	FGsRemoveItemList* paramData = static_cast<FGsRemoveItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	TArray<TPair<ItemType, ItemDBId>> removeDataList;
	for (TPair<EInventoryTab, TArray<TPair<ItemType, ItemDBId>>> iter : _invenItemDbidMap)
	{
		TArray<TPair<ItemType, ItemDBId>> dbidList = iter.Value;
		for (TPair<ItemType, ItemDBId> pairData : dbidList)
		{
			for (ItemDBId removeItemDBId : paramData->_itemDBIdList)
			{
				if (pairData.Value == removeItemDBId)
				{
					removeDataList.Emplace(pairData);
				}
			}
		}
	}
	if (0 < removeDataList.Num())
	{
		for (TPair<ItemType, ItemDBId> removePiarList : removeDataList)
		{
			for (TPair<EInventoryTab, TArray<TPair<ItemType, ItemDBId>>>& iter : _invenItemDbidMap)
			{
				TArray<TPair<ItemType, ItemDBId>>& pairDataList = iter.Value;
				if (INDEX_NONE != pairDataList.Find(removePiarList))
				{
					pairDataList.Remove(removePiarList);
					continue;
				}
			}
		}
		removeDataList.Empty();
	}

	UpdateItemWeightRate();

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->RemoveItemList(paramData->_itemDBIdList);
	}
}

void UGsInventoryManager::OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount)
{
	_inventorySlotCount = InMaxSlotCount;
	_totalExtendCount = InTotalExtendCount;

	FText resultText;
	FText findText;
	if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_FullExtend"), findText))
	{
		resultText = findText.Format(findText, _totalExtendCount);
		FGsUIHelper::TrayMessageTicker(resultText);
	}

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->OnChangeInvenSlotCount(_inventorySlotCount, _totalExtendCount);
	}
}

void UGsInventoryManager::OnChangeIconDimmed(struct FGsItemMessageParamBase& InParam)
{
	FGsReactionItemLock* paramData = static_cast<FGsReactionItemLock*>(&InParam);
	if (nullptr == paramData)
		return;

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->SetItemDimmed(paramData->_isLock, paramData->_lockItemCategorySubList);
	}
}

void UGsInventoryManager::UpdateInvenWeightRate()
{
	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}
	int32 ownedItemTotalWeight = _itemMgr->GetOwnedItemWeight();
	float weightPercent = FGsItemHelper::GetWeightPercent(ownedItemTotalWeight);

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->SetWeightPercentInfo(ownedItemTotalWeight, weightPercent);
	}
}

void UGsInventoryManager::UpdateItemWeightRate()
{
	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}
	int32 ownedItemTotalWeight = _itemMgr->GetOwnedItemWeight();
	float weightPercent = FGsItemHelper::GetWeightPercent(ownedItemTotalWeight);

	// 무게 및 내림처리된 퍼센트(%) 정보 Send
	FGsUpdateInvenWeightInfo param(ownedItemTotalWeight, weightPercent);
	GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::UPDATE_ITEM_WEIGHT, &param);

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->SetWeightPercentInfo(ownedItemTotalWeight, weightPercent);
	}
}

void UGsInventoryManager::SendPenaltyInfoFromPacketData(WeightPenaltyGrade InPenaltyGradeType)
{
	FGsUpdateInvenWeightPenaltyType param(InPenaltyGradeType);
	GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::UPDATE_ITEM_WEIGHT_PENALTY, &param);

	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->SetWeightPenaltyType(InPenaltyGradeType);
	}
}

bool UGsInventoryManager::IsOpenInventoryPopup() const
{
	if (UGsUIManager* uiManager = GUI())
	{
		return uiManager->IsActiveWidget(TEXT("PopupInventory"));
	}
	return false;
}

bool UGsInventoryManager::FindInvenUIptr()
{
	if (false == _invenUIPtr.IsValid())
	{
		if (UGsUIManager* uiManager = GUI())
		{
			if (uiManager->IsActiveWidget(TEXT("PopupInventory")))
			{
				TWeakObjectPtr<UGsUIWidgetBase> invenUI = uiManager->OpenAndGetWidget("PopupInventory");
				if (invenUI.IsValid())
				{
					_invenUIPtr = Cast<UGsUIPopupInventory>(invenUI);
				}
			}
		}
	}
	return _invenUIPtr.IsValid();
}

void UGsInventoryManager::OnChangeInvenIndex(const struct IGsMessageParam* inParam)
{
	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->OnChangeSlotIndex();
	}
}

TArray<ItemDBId> UGsInventoryManager::GetDisplayItemList(const EInventoryTab InTabType)
{
	UGsItemManager* itemMgr =  GItem();
	TArray<TPair<ItemType, ItemDBId>> removeDataList;
	TArray<ItemDBId> removeList;
	TArray<ItemDBId> displayList;
	if (EInventoryTab::AllTab == InTabType)
	{
		for (TPair<EInventoryTab, TArray<TPair<ItemType, ItemDBId>>> iter : _invenItemDbidMap)
		{
			for (TPair<ItemType, ItemDBId> pairData : iter.Value)
			{
				TWeakPtr<FGsItem> findData = itemMgr->FindItem(pairData.Value, pairData.Key);
				if (findData.IsValid())
				{
					displayList.Emplace(pairData.Value);
				}
				else
				{
					removeDataList.Emplace(pairData);
				}
			}
		}
	}
	else
	{
		if (_invenItemDbidMap.Contains(InTabType))
		{
			TArray<TPair<ItemType, ItemDBId>> itemList = _invenItemDbidMap[InTabType];
			for (TPair<ItemType, ItemDBId> pairData : itemList)
			{
				TWeakPtr<FGsItem> findData = itemMgr->FindItem(pairData.Value, pairData.Key);
				if (findData.IsValid())
				{
					displayList.Emplace(pairData.Value);
				}
				else
				{
					removeDataList.Emplace(pairData);
				}
			}
		}
	}
	
	if (0 < removeDataList.Num())
	{
		for (TPair<ItemType, ItemDBId> removePiarList : removeDataList)
		{
			for (TPair<EInventoryTab, TArray<TPair<ItemType, ItemDBId>>>& iter : _invenItemDbidMap)
			{
				if (INDEX_NONE != iter.Value.Find(removePiarList))
				{
					iter.Value.Remove(removePiarList);
					continue;
				}
			}
		}

		removeDataList.Empty();
	}

	itemMgr->SortItemListByDBID(UGsUIPopupInventory::ESortUIList::ServerSlotType, displayList);

	return displayList;
}

void UGsInventoryManager::OnChangeInvenTab(EInventoryTab InTabType)
{
	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->CreateItemList(GetDisplayItemList(InTabType), _itemMgr->GetReactionLockFlag());
	}
}

// 인벤토리 종료할때 같이 종료 닫혀야 하는것들 처리
void UGsInventoryManager::CloseInventoryPopup(bool InIsCallUI, const bool InIsNeedRecord)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;

	if (false == InIsCallUI)
	{
		if (uiMgr->IsActiveWidget(TEXT("PopupInventory")))
		{
			uiMgr->CloseByKeyName(TEXT("PopupInventory"));
		}	
	}
	else
	{
		if (FGsMessageHolder* msgMgr = GMessage())
		{
			msgMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::INVEN, false);
		}

		if (UGsQuickSlotManager* quickSlotMgr = GSQuickSlot())
		{
			quickSlotMgr->CloseInven();
		}

		if (UGsItemManager* itemMgr = GItem())
		{
			itemMgr->TrySendInvenSlotIndex(GetDisplayItemList(EInventoryTab::AllTab), InIsNeedRecord);
		}

		_invenUIPtr.Reset();
	}

	if (uiMgr->IsActiveWidget(TEXT("PopupCommonPay")))
	{
		uiMgr->CloseByKeyName(TEXT("PopupCommonPay"));
	}
}

void UGsInventoryManager::OnPlayerStatUpdate(const struct IGsMessageParam* inParam)
{
	if (nullptr == inParam)
		return;

	const FGsGameObjectMessageParamPlayerStat* param = inParam->Cast<const FGsGameObjectMessageParamPlayerStat>();
	if (nullptr == param)
		return;

	//TArray<TPair<StatType, int32>>
	for (TPair<StatType, int32> iter : param->StatMap)
	{
		if (StatType::MAX_WEIGHT == iter.Key || StatType::WEIGHT == iter.Key)
		{
			UpdateItemWeightRate();
			break;
		}
	}
}

void UGsInventoryManager::OnUnSelect()
{
	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		_invenUIPtr->ResetIconSelection();
	}
}

void UGsInventoryManager::LoadConfigTable()
{
	const UGsTableInventoryConfig* table = Cast<UGsTableInventoryConfig>(FGsSchemaInventoryConfig::GetStaticTable());
	if (nullptr == table)
		return;

	TArray<const FGsSchemaInventoryConfig*> rowList;
	if (false == table->GetAllRows(rowList))
		return;

	if (0 >= rowList.Num())
		return;

	_invenConfigData = rowList[0];
}

uint8 UGsInventoryManager::GetEnoughExtendSlotCount()
{
	if (nullptr == _invenConfigData)
	{
		LoadConfigTable();
		if (nullptr == _invenConfigData)
		{
			check(_invenConfigData);
			return 0;
		}
	}

	if (_totalExtendCount >= (uint32)_invenConfigData->extendMaxCount)
	{
		return 0;
	}

	return _invenConfigData->extendMaxCount - _totalExtendCount;
}

uint16 UGsInventoryManager::GetMaxinumSlotLineCount()
{
	if (nullptr == _invenConfigData)
	{
		LoadConfigTable();
		if (nullptr == _invenConfigData)
		{
			check(_invenConfigData);
			return 0;
		}
	}

	return _invenConfigData->extendMaxCount;
}

uint16 UGsInventoryManager::GetTotalMaxinumSlotLineCount()
{
	uint16 calc = DEFAULT_INVENTORY_SLOT / _invenConfigData->extendSlotCount;
	uint16 result = calc + GetMaxinumSlotLineCount();

	return result;
}

uint16 UGsInventoryManager::GetTotalMaxinumSlotCount()
{
	uint16 calc1 = _invenConfigData->extendMaxCount * _invenConfigData->extendSlotCount;
	uint16 result = DEFAULT_INVENTORY_SLOT + calc1;

	return result;
}

CurrencyType UGsInventoryManager::GetExtendSlotCurrencyType()
{
	if (nullptr == _invenConfigData)
	{
		LoadConfigTable();
		if (nullptr == _invenConfigData)
		{
			check(_invenConfigData);
			return CurrencyType::NONE;
		}
	}

	return _invenConfigData->extendSlotCurrencyType;
}

int32 UGsInventoryManager::GetExtendSlotCost()
{
	if (nullptr == _invenConfigData)
	{
		LoadConfigTable();
		if (nullptr == _invenConfigData)
		{
			check(_invenConfigData);
			return 0;
		}
	}

	return _invenConfigData->extendSlotCurrencyCost;
}

int32 UGsInventoryManager::GetOwnedItemPenaltyValueByType(const WeightPenaltyGrade InPenaltyGrade)
{
	if (nullptr == _invenConfigData)
	{
		LoadConfigTable();
		if (nullptr == _invenConfigData)
		{
			check(_invenConfigData);
			return 0;
		}
	}

	switch (InPenaltyGrade)
	{
	case WeightPenaltyGrade::FIRST:
		return _invenConfigData->weightPenalty1Phase / 100;
	case WeightPenaltyGrade::SECOND:
		return _invenConfigData->weightPenalty2Phase / 100;
	}

	return 0;
}

bool UGsInventoryManager::IsEnoughExtendSlotCost()
{
	uint64 curValue = FGsCurrencyHelper::GetCurrencyAmount(GetExtendSlotCurrencyType());
	if (curValue >= GetExtendSlotCost())
	{
		return true;
	}
	return false;
}

void UGsInventoryManager::SendExtendSlot(uint8 InExtendLevel)
{
	if (true == IsEnoughExtendSlotCost())
	{
		FGsNetSendServiceItem::SendExtendInvenSlot(InExtendLevel);
	}
}

bool UGsInventoryManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}
	bool IsReddot = _itemMgr->CheckRedDotItemByItemType(ItemType::EQUIP)
		|| _itemMgr->CheckRedDotItemByItemType(ItemType::CONSUME)
		|| _itemMgr->CheckRedDotItemByItemType(ItemType::INGREDIENT)
		|| _itemMgr->CheckRedDotItemByItemType(ItemType::CURRENCY);

	SetRedDot(IsReddot);

	return IsRedDot();
}

uint32 UGsInventoryManager::GetInvenSlotMaxCountWithTemp()
{
	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}
	uint16 ownedCount = _itemMgr->GetOwnedItemCount();
	if (ownedCount > _inventorySlotCount)
	{
		if (nullptr == _invenConfigData)
		{
			LoadConfigTable();
		}
		if (_invenConfigData)
		{
			if (0 != (ownedCount % _invenConfigData->extendSlotCount))
			{
				uint16 calc1 = ownedCount / _invenConfigData->extendSlotCount;
				uint16 result = (calc1 + 1) * _invenConfigData->extendSlotCount;
				return result;
			}
		}
		return ownedCount;
	}
	return _inventorySlotCount;
}

bool UGsInventoryManager::IsUseLongPress()
{
	// 롱프레스 옵션 체크
	if (UGsGameUserSettings* userSetting = GGameUserSettings())
	{
		uint8 value = userSetting->GetCombatSetting(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE);
		return (0 == value) ? false : true;
	}
	return false;
}

bool UGsInventoryManager::IsReactionLockTypeItem(const ItemCategorySub InSubType)
{
	if (false == _itemMgr.IsValid())
	{
		_itemMgr = GItem();
	}
	return _itemMgr->IsReactionLockTypeItem(InSubType);
}

ItemDBId UGsInventoryManager::GetSelectionItemDbid()
{
	if (IsOpenInventoryPopup() && FindInvenUIptr())
	{
		return _invenUIPtr->GetSelectionItemDbid();
	}
	return 0;
}

// 인게임 로딩중에, 각종 인벤토리에서 사용할 itemiconSlot Widget 을 미리 생성하여 pool 처리함
void UGsInventoryManager::WarmUpInvenIconSlot()
{
	FGsIconFactory::WarmUpIcon(EGsIconType::ICON_ITEM_INVENTORY_XL, GetInvenSlotMaxCountWithTemp());
}

void UGsInventoryManager::InvalidatePlayerStat()
{
	UpdateItemWeightRate();
}

bool UGsInventoryManager::IsInventoryOpen() const
{
	if (UGsUIManager* uiMgr = GUI())
	{
		return uiMgr->IsActiveWidget(TEXT("PopupInventory"));
	}

	return false;
}

bool UGsInventoryManager::CheckIsHUDState() const
{
	if (GMode() != nullptr)
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsHud)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void UGsInventoryManager::RestoreInventoryAndDetailPopup()
{
#ifdef RESTORE_INVENTORY
	if (INVALID_ITEM_DB_ID == _restoreItemDBId)
	{
		return;
	}

	// HUD 상태에서만 복구함
	if (false == CheckIsHUDState())
	{
		return;
	}

	// 튜토리얼 아닐 때만 인벤토리 복구 진행
	if (FGsTutorialManager* tutorialMgr = GSTutorial())
	{
		if (tutorialMgr->IsActiveTutorial())
		{
			SetRestoreItemDBId(INVALID_ITEM_DB_ID);
			return;
		}
	}

	// 인벤토리 복구
	if (RestoreInventory())
	{
		// 인벤복구 성공, 아이템이 유효하면 상세창도 복구
		if (UGsItemManager* itemMgr = GItem())
		{
			TWeakPtr<FGsItem> itemPtr = itemMgr->FindItem(_restoreItemDBId);
			if (itemPtr.IsValid())
			{
				if (FGsMessageHolder* msgMgr = GMessage())
				{
					FGsItemDetailOpenMessageParamEX::DetailOption option;
					option._bHideBG = true;
					option._bUseCollectionMarking = true;

					FGsItemDetailOpenMessageParamEX param(itemPtr.Pin().Get(), FGsItemDetailOpenMessageParamEX::PosDir::RightPos, option);
					msgMgr->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
				}
			}
		}
	}

	SetRestoreItemDBId(INVALID_ITEM_DB_ID);
#endif
}

bool UGsInventoryManager::RestoreInventory()
{
#ifdef RESTORE_INVENTORY
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return false;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return false;
	}

	if (uiManager->IsActiveWidget(TEXT("PopupInventory")))
	{
		return false;
	}

	// 아이템이 사라졌으면 인벤은 열어주고 false 리턴
	TWeakPtr<FGsItem> itemPtr = itemMgr->FindItem(_restoreItemDBId);
	if (false == itemPtr.IsValid())
	{
		OpenInventory(EInventoryTab::AllTab);
		return false;
	}

	OpenInventory(EInventoryTab::AllTab);

	if (_invenUIPtr.IsValid())
	{
		_invenUIPtr->ScrollToTargetItem(_restoreItemDBId);
	}

	return true;
#else
	return false;
#endif
}


/*
void UGsInventoryManager::CloseEnchantPopup()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (true == uiManager->IsActiveWidget(TEXT("PopupItemEnchant")))
		{
			GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::CLOSE, nullptr);
		}
	}
}

void UGsInventoryManager::CloseMultiDecomposePopup()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (true == uiManager->IsActiveWidget(TEXT("PopupItemDecompose")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDecompose"));
		}
	}
}
*/
