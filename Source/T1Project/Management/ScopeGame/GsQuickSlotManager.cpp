// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuickSlotManager.h"
#include "T1Project.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDQuickSlot.h"

#include "Sound/GsSoundPlayer.h"


void UGsQuickSlotManager::Initialize()
{
	_quickSlotInfoMap.Empty();
	for (uint8 i = 0; i < MAX_QUICK_SLOT_SIZE; ++i)
	{
		QuickSlotInfo* slotInfo = new QuickSlotInfo();
		_quickSlotInfoMap.Emplace(i+1, slotInfo);
	}

	_fromInvenSelectionDBID = 0;
	_arrSelectionQuickSlotID[0] = 0;
	_arrSelectionQuickSlotID[1] = 0;
}

void UGsQuickSlotManager::Finalize()
{
	for (auto& iter : _quickSlotInfoMap)
	{
		delete iter.Value;
		iter.Value = nullptr;
	}
	_quickSlotInfoMap.Empty();

	_lastSlotPage = 0;
	_hudQuickSlotUI.Reset();
}

void UGsQuickSlotManager::SetPacketData(QuickSlotId InslotId, ItemDBId InItemDBID, ItemId InItemTID, bool isAutoState)
{
	if (_quickSlotInfoMap.Contains(InslotId))
	{
		QuickSlotInfo& infoData = *_quickSlotInfoMap[InslotId];
		infoData.mItemDBId = InItemDBID;
		infoData.mTemplateId = InItemTID;
		infoData.mAutoUse = isAutoState;
	}
	else
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::SetPacketData] InslotId is WrongID : %d"), InslotId);
	}
}

void UGsQuickSlotManager::SetPacketData(QuickSlotId InslotId, ItemDBId InItemDBID, bool isAutoState)
{
	if (_quickSlotInfoMap.Contains(InslotId))
	{
		QuickSlotInfo& infoData = *_quickSlotInfoMap[InslotId];
		infoData.mItemDBId = InItemDBID;
		infoData.mAutoUse = isAutoState;
	}
	else
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::SetPacketData] InslotId is WrongID : %d"), InslotId);
	}
}

const QuickSlotInfo* UGsQuickSlotManager::GetQuickSlotInfoBySlotID(const QuickSlotId InSlotID)
{
	if (_quickSlotInfoMap.Contains(InSlotID))
	{
		return _quickSlotInfoMap[InSlotID];
	}

	return nullptr;
}

bool UGsQuickSlotManager::IsOwnedItemBySlotID(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::IsOwnedItemSlot] InslotId is WrongID : %d"), InSlotID);
		return false;
	}

	uint32 findID = findSlotInfo->mTemplateId;
	TArray<TWeakPtr<FGsItem>> findItemList = GItem()->FindByTID(findID);
	return 0 < findItemList.Num();
}

bool UGsQuickSlotManager::IsOwnedItemByItemDBID(const ItemDBId InItemDBID)
{
	TWeakPtr<FGsItem> itemData = GItem()->FindItem(InItemDBID);
	if (false == itemData.IsValid())
		return false;

	for (const auto& iter : _quickSlotInfoMap)
	{
		if (nullptr == iter.Value) // slotInfo : iter.Value
			continue;

		if (iter.Value->mItemDBId == InItemDBID)
		{
			return true;
		}
	}

	return false;
}

bool UGsQuickSlotManager::IsExistSlotItemByItemTID(const int32 InItemTID)
{
	if (0 >= InItemTID)
		return false;

	for (const auto& iter : _quickSlotInfoMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (iter.Value->mTemplateId == InItemTID)
		{
			return true;
		}
	}

	return false;
}

bool UGsQuickSlotManager::IsAllowedAutoUseItem(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::IsAllowedAutoItem] InslotId is WrongID : %d"), InSlotID);
		return false;
	}

	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(findSlotInfo->mTemplateId);
	if (nullptr == commonData)
		return false;

	if (ItemType::CONSUME != commonData->type)
		return false;

	const UGsTableItemConsumeDetail* table = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaItemConsumeDetail* row = nullptr;
	if (false == table->FindRowById(commonData->detailId, row))
	{
		return false;
	}

	return row->isAllowedAutoUse;
}

bool UGsQuickSlotManager::IsDimmedRegistItem(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::IsDimmedRegistItem] InslotId is WrongID : %d"), InSlotID);
		return false;
	}

	return 0 < findSlotInfo->mTemplateId;
}

const UGsQuickSlotManager::SlotRegisterItemStateType UGsQuickSlotManager::GetSlotRegisterStateType(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::GetSlotRegisterStateType] InslotId is WrongID : %d"), InSlotID);
		return SlotRegisterItemStateType::Empty;
	}

	if (true == IsOwnedItemBySlotID(InSlotID))
		return SlotRegisterItemStateType::OwnedItem;
	if (true == IsDimmedRegistItem(InSlotID))
		return SlotRegisterItemStateType::OnlyItem;
	
	return SlotRegisterItemStateType::Empty;
}

void UGsQuickSlotManager::FindAllSlotIdByItemTID(const int32 InItemTID, OUT TArray<uint8>& outSlotIDList)
{
	for (const auto& iter : _quickSlotInfoMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (iter.Value->mTemplateId == InItemTID)
		{
			outSlotIDList.Emplace(iter.Key);
		}
	}
}

void UGsQuickSlotManager::FindAllSlotIdByItemDBID(const uint64 InItemDBID, OUT TArray<uint8>& outSlotIDList)
{
	for (const auto& iter : _quickSlotInfoMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (iter.Value->mItemDBId == InItemDBID)
		{
			outSlotIDList.Emplace(iter.Key);
		}
	}
}

// 퀵슬롯에 등록은 되어있지만, 실제로 보유하지 않은 아이템
void UGsQuickSlotManager::FindAllNotOwnedSlotIdByItemTID(const int32 InItemTID, OUT TArray<uint8>& outSlotIDList)
{
	for (const auto& iter : _quickSlotInfoMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (iter.Value->mTemplateId == InItemTID && 0 >= iter.Value->mItemDBId)
		{
			outSlotIDList.Emplace(iter.Key);
		}
	}
}

bool UGsQuickSlotManager::IsConsumableItem(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::IsConsumableItem] InslotId is WrongID : %d"), InSlotID);
		return false;
	}
	
	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(findSlotInfo->mTemplateId);
	if (nullptr == commonData)
		return false;
	return ItemType::CONSUME == commonData->type;
}

bool UGsQuickSlotManager::IsIngredientItem(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::IsIngredientItem] InslotId is WrongID : %d"), InSlotID);
		return false;
	}

	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(findSlotInfo->mTemplateId);
	if (nullptr == commonData)
		return false;
	return ItemType::INGREDIENT == commonData->type;
}

bool UGsQuickSlotManager::IsEquippedItem(const QuickSlotId InSlotID)
{
	const QuickSlotInfo* findSlotInfo = GetQuickSlotInfoBySlotID(InSlotID);
	if (nullptr == findSlotInfo)
	{
		GSLOG(Warning, TEXT("[UGsQuickSlotManager::IsEquippedItem] InslotId is WrongID : %d"), InSlotID);
		return false;
	}

	UGsItemManager* itemMgr = GItem();
	TWeakPtr<FGsItem> itemData = itemMgr->FindItem(findSlotInfo->mItemDBId);
	if (false == itemData.IsValid())
	{
		return false;
	}
	return itemMgr->IsCurrentPresetItem(itemData.Pin().Get());
}

// 장비는 DBID 기준 제거처리 , 이외는 tid 기준 제거처리
void UGsQuickSlotManager::ResetQuickSlotOwnedItemInfo(const uint64 InRemoveItemDBID, const int32 InItemTID, OUT TArray<uint8>& outSlotIDList)
{
	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == commonData)
		return;

	for (auto& iter : _quickSlotInfoMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (ItemType::EQUIP == commonData->type)
		{
			if (iter.Value->mItemDBId == InRemoveItemDBID)
			{
				iter.Value->mItemDBId = 0;
				outSlotIDList.Emplace(iter.Key);
			}
		}
		else
		{
			if (iter.Value->mTemplateId == InItemTID)
			{
				iter.Value->mItemDBId = iter.Value->mItemDBId == InRemoveItemDBID ? 0 : iter.Value->mItemDBId;
				outSlotIDList.Emplace(iter.Key);
			}
		}
	}
}

void UGsQuickSlotManager::ResetAllSelectionInfo()
{
	_fromInvenSelectionDBID = 0;
	_arrSelectionQuickSlotID[0] = 0;
	_arrSelectionQuickSlotID[1] = 0;
	if (isValidUI())
	{
		_hudQuickSlotUI->OnChangeQuickSlotEditEffectState(false);
		_hudQuickSlotUI->ResetAllSelectionInfo();
	}
	GSInventory()->OnUnSelect();
}

void UGsQuickSlotManager::ResetSelectionInfo(const QuickSlotId InSlotId)
{
	//_fromInvenSelectionDBID = 0;
	_arrSelectionQuickSlotID[0] = 0;
	_arrSelectionQuickSlotID[1] = 0;
	if (isValidUI())
	{
		_hudQuickSlotUI->OnChangeQuickSlotEditEffectState(IsEditMode());
		_hudQuickSlotUI->QuickSlotInvalidateBySlotIndex(TArray<QuickSlotId> { InSlotId });
		_hudQuickSlotUI->ResetSelectionInfo(InSlotId);
	}
}

void UGsQuickSlotManager::ResetQuickSlotSelectionInfo()
{
	_arrSelectionQuickSlotID[0] = 0;
	_arrSelectionQuickSlotID[1] = 0;
}

void UGsQuickSlotManager::CloseInven()
{
	_fromInvenSelectionDBID = 0;
	_arrSelectionQuickSlotID[0] = 0;
	_arrSelectionQuickSlotID[1] = 0;
	if (isValidUI())
	{
		_hudQuickSlotUI->OnChangeQuickSlotEditEffectState(false);
		_hudQuickSlotUI->ResetAllSelectionInfo();
	}
}

void UGsQuickSlotManager::OnRemoveInvenItem(const ItemDBId InItemDbid)
{
	if (_fromInvenSelectionDBID == InItemDbid)
	{
		_fromInvenSelectionDBID = 0;
		if (isValidUI())
		{
			_hudQuickSlotUI->OnChangeQuickSlotEditEffectState((0 < _arrSelectionQuickSlotID[0] || 0 < _fromInvenSelectionDBID));
		}
	}
}

void UGsQuickSlotManager::CheckSelectionInfo()
{
	// 1. 인벤 -> 퀵슬롯 등록 (무조건 덮어씀)
	// 2. 퀵슬롯 -> 인벤 (1과 동일)
	if (0 < _fromInvenSelectionDBID && 0 < _arrSelectionQuickSlotID[0])
	{
		// register
		FGsRegisterQuickSlot param(_fromInvenSelectionDBID, _arrSelectionQuickSlotID[0]);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_REGISTER, &param);
		ResetAllSelectionInfo();
	}
	// 3. 퀵슬롯 -> 퀵슬롯 (swap)
	else if (0 < _arrSelectionQuickSlotID[0] && 0 < _arrSelectionQuickSlotID[1])
	{
		if (_arrSelectionQuickSlotID[0] == _arrSelectionQuickSlotID[1])
		{
			// 퀵슬롯 아이템 해제
			FGsOnClickRemoveQuickSlotIcon paramData(_arrSelectionQuickSlotID[0]);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_REMOVE, &paramData);

			// 인벤토리가 열린 상태에서 퀵슬롯 아이템을 사용하게 만들고 싶을 경우 주석 해제
			//UGsQuickSlotManager::SlotRegisterItemStateType  type = GetSlotRegisterStateType(_arrSelectionQuickSlotID[0]);
			//if (type == UGsQuickSlotManager::SlotRegisterItemStateType::OwnedItem)
			//{
			//	FGsOnClickUseQuickSlotItem paramData(_arrSelectionQuickSlotID[0]);
			//	GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_USE, &paramData);
			//}
		}
		else
		{
			// swap
			FGsSwapQuickSlot param(_arrSelectionQuickSlotID[0], _arrSelectionQuickSlotID[1]);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_SWAP, &param);
		}
		ResetAllSelectionInfo();
	}
}

void UGsQuickSlotManager::SetInvenSelectionItem(const uint64 InItemDBID)
{
	_fromInvenSelectionDBID = InItemDBID;
	_hudQuickSlotUI->OnChangeQuickSlotEditEffectState(IsEditMode());
	CheckSelectionInfo();
}

void UGsQuickSlotManager::SetQuickSlotSelectionItem(const QuickSlotId InSlotID, const bool InIsEmptySlot)
{
	if (0 >= _arrSelectionQuickSlotID[0])
	{
		if (InIsEmptySlot && 0 >= _fromInvenSelectionDBID)
		{
			return;
		}
		_arrSelectionQuickSlotID[0] = InSlotID;
	}
	else
	{
		_arrSelectionQuickSlotID[1] = InSlotID;
	}
	_hudQuickSlotUI->OnChangeQuickSlotEditEffectState(IsEditMode());
}

bool UGsQuickSlotManager::SetAutoState(const QuickSlotId InSlotID)
{
	if (false == _quickSlotInfoMap.Contains(InSlotID))
		return false;

	if (false == IsAllowedAutoUseItem(InSlotID))
		return false;

	QuickSlotInfo* slotInfo = _quickSlotInfoMap[InSlotID];
	if (nullptr == slotInfo)
		return false;

	bool curState = slotInfo->mAutoUse;
	uint64 dbid = slotInfo->mItemDBId;
	int32 tid = slotInfo->mTemplateId;

	FGsOnChangeQuickSlotAutoState param(InSlotID, dbid, !curState);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_AUTOSTATE, &param);

	return true;
}

bool UGsQuickSlotManager::isValidUI()
{
	if (false == _hudQuickSlotUI.IsValid())
	{
		UGsUIManager* uiMgr = GUI();
		if (uiMgr)
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(TEXT("HUDMain"));
			if (widget.IsValid())
			{
				UGsUIHUDFrameMain* frameMain = Cast<UGsUIHUDFrameMain>(widget.Get());
				_hudQuickSlotUI = frameMain->GetQuickSlot();
			}
		}
	}
	return _hudQuickSlotUI.IsValid();
}

void UGsQuickSlotManager::CompleteLoadUI()
{
	isValidUI();
}

void UGsQuickSlotManager::InvalidateAll()
{
	if (isValidUI())
	{
		_hudQuickSlotUI->QuickSlotInvalidateAll();
	}
}

void UGsQuickSlotManager::InvalidateSlotList(TArray<QuickSlotId>& InInvalidateSlotIDList)
{
	if (isValidUI())
	{
		_hudQuickSlotUI->QuickSlotInvalidateBySlotIndex(InInvalidateSlotIDList);
	}
}

void UGsQuickSlotManager::InvalidateAutoState(TArray<QuickSlotId>& InSlotIdList)
{
	if (isValidUI())
	{
		_hudQuickSlotUI->InvalidateAutoState(InSlotIdList);
	}
}

void UGsQuickSlotManager::CheckSound(const bool InIsEquipped)
{
	FString fileName = InIsEquipped ? "UI_EquipGear" : "UI_Cancel";
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr != soundPlayer)
	{
		soundPlayer->PlaySound2D(fileName);
	}
}

const bool UGsQuickSlotManager::IsEditMode()
{
	UGsUIManager* uiManager = GUI();
	if (uiManager == nullptr)
		return false;

	return ((uiManager->IsActiveWidget(TEXT("PopupInventory"))) && (0 < _fromInvenSelectionDBID || 0 < _arrSelectionQuickSlotID[0]));
}