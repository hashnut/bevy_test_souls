


#include "GsInvadeBattleLevelManager.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageInvasion.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "GsTable.h"

#include "DataSchema/InterServer/GsSchemaInvadeConfig.h"
#include "DataSchema/InterServer/GsSchemaInvadeBattleLevel.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldConfigDataEx.h"
#include "T1Project.h"
#include "../UTIL/GsTableUtil.h"

void FGsInvadeBattleLevelManager::Initialize()
{
	LoadBattleLevel();

	RegisterMessage();

	const FGsSchemaInvadeWorldConfigDataEx* configTable = GetInvadeWorldConfigDataEx();
	if (configTable)
	{
		_attackCurrencyItemId = (configTable->invadeAttackCurrencyItemId.GetRow()) ? configTable->invadeAttackCurrencyItemId.GetRow()->id : INVALID_ITEM_ID;
		_defenceCurrencyItemId = (configTable->invadeDefenceCurrencyItemId.GetRow()) ? configTable->invadeDefenceCurrencyItemId.GetRow()->id : INVALID_ITEM_ID;
	}
}

void FGsInvadeBattleLevelManager::Finalize()
{
	_battleLevelHolder.Empty();


	UnRegisterMessage();
}

void FGsInvadeBattleLevelManager::LoadBattleLevel()
{
	const UGsTable* table = FGsSchemaInvadeBattleLevel::GetStaticTable();
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return;
	}

	const TMap<FName, uint8*>* OutRowMap;
	if (false == table->GetRowMap(OutRowMap))
	{
		GSLOG(Error, TEXT("false == table->GetRowMap(OutRowMap)"));
		return;
	}

	_battleLevelHolder.Reserve(OutRowMap->Num());

	for (auto& RowKvIt : *OutRowMap)
	{
		const FGsSchemaInvadeBattleLevel* schema = reinterpret_cast<const FGsSchemaInvadeBattleLevel*>(RowKvIt.Value);
		_battleLevelHolder.Emplace(schema);
	}

	_battleLevelHolder.Sort([](const FGsSchemaInvadeBattleLevel& lhr, const FGsSchemaInvadeBattleLevel& rhr)
		{
			return lhr.battleLevel < rhr.battleLevel;
		}
	);
}

void FGsInvadeBattleLevelManager::RegisterMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (messageHolder)
	{
		// 2022/08/24 PKT - Local Spawn Spawn
		_listGameObjectDelegate.Emplace(messageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE
			, this
			, &FGsInvadeBattleLevelManager::OnLocalPlayerSpawnComplete));

		// 2021/08/02 PKT - inventory Item - New ..
		_listItemActionDelegates.Emplace(
			messageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsInvadeBattleLevelManager::OnAddItem)
		);
		// 2021/08/02 PKT - inventory Item - Update ..
		_listItemActionDelegates.Emplace(
			messageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsInvadeBattleLevelManager::OnUpdateItem)
		);
		// 2021/08/02 PKT - inventory Item - Remove ..
		_listItemActionDelegates.Emplace(
			messageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsInvadeBattleLevelManager::OnRemoveItem)
		);
	}
}

void FGsInvadeBattleLevelManager::UnRegisterMessage()
{
	FGsMessageHolder* messageHolder = GMessage();

	if (messageHolder)
	{
		for (auto& item : _listGameObjectDelegate)
		{
			messageHolder->GetGameObject().Remove(item);
		}
		_listGameObjectDelegate.Empty();

		for (auto& item : _listItemActionDelegates)
		{
			messageHolder->GetItem().Remove(item);
		}
		_listItemActionDelegates.Empty();
	}
}

void FGsInvadeBattleLevelManager::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam)
{

}

void FGsInvadeBattleLevelManager::OnAddItem(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();

	for (int32 idx = 0; idx < param->_itemDbidList.Num(); ++idx)
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(param->_itemDbidList[idx]);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), param->_itemDbidList[idx]);
			return;
		}

		SendMessageIfValidCostItem(item.Pin()->GetTID());
	}
}

void FGsInvadeBattleLevelManager::OnUpdateItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(Param);

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	if (itemManager.IsValid())
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(Param->_itemDBID);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}
		SendMessageIfValidCostItem(item.Pin()->GetTID());
	}
}

void FGsInvadeBattleLevelManager::OnRemoveItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);
	SendMessageIfValidCostItem(Param->_itemTID);
}

void FGsInvadeBattleLevelManager::SendMessageIfValidCostItem(const ItemId InItemId)
{
	if (_attackCurrencyItemId == InItemId || _defenceCurrencyItemId == InItemId)
	{
		// 2022/05/25 PKT - UI 갱신 요청
		SendMessage();
	}
}

void FGsInvadeBattleLevelManager::SendMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (messageHolder)
	{
		messageHolder->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_USER_COUNT, nullptr);
	}
}

const TArray<const struct FGsSchemaInvadeBattleLevel*>& FGsInvadeBattleLevelManager::ViewList() const
{
	return _battleLevelHolder;
}

CurrencyDelta FGsInvadeBattleLevelManager::GetAttackCurrencyItemCount() const
{
	CurrencyDelta outAmount = 0;
	for (const auto& item : GItem()->FindByTID(_attackCurrencyItemId))
	{
		outAmount += item.Pin().Get()->GetAmount();
	}
	
	return outAmount;
}

CurrencyDelta FGsInvadeBattleLevelManager::GetDefenceCurrencyItemCount() const
{
	CurrencyDelta outAmount = 0;
	for (const auto& item : GItem()->FindByTID(_defenceCurrencyItemId))
	{
		outAmount += item.Pin().Get()->GetAmount();
	}

	return outAmount;
}

const FGsSchemaInvadeBattleLevel* FGsInvadeBattleLevelManager::FindBattleLevelInfo(InvadeBattleLevel InLevel) const
{
	for (const FGsSchemaInvadeBattleLevel* item : _battleLevelHolder)
	{
		if (item->battleLevel == InLevel)
		{
			return item;
		}
	}

	return nullptr;
}

const FGsSchemaInvadeConfig* FGsInvadeBattleLevelManager::GetConfigData()
{
	const UGsTable* table = FGsSchemaInvadeConfig::GetStaticTable();
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("nullptr == table"));
		return nullptr;
	}

	const TMap<FName, uint8*>* OutRowMap;
	if (false == table->GetRowMap(OutRowMap))
	{
		GSLOG(Error, TEXT("false == table->GetRowMap(OutRowMap)"));
		return nullptr;
	}

	// 2022/06/09 PKT - Config Data는 한개만 존재해야 한다.
	const static int32 CONFIG_DATA_ONLY_ONE = 1;
	if (CONFIG_DATA_ONLY_ONE > OutRowMap->Num())
	{
		GSLOG(Error, TEXT("CONFIG_DATA_ONLY_ONE > OutRowMap->Num()"));
		return nullptr;
	}

	const FGsSchemaInvadeConfig* config = nullptr;
	for (const auto& pair : (*OutRowMap))
	{	// 2022/06/09 PKT - 하나만 적용 시키고 빠져 나간다.
		config = reinterpret_cast<const FGsSchemaInvadeConfig*>(pair.Value);
		if (nullptr != config)
		{
			break;
		}
	}

	return config;
}

const struct FGsSchemaInvadeWorldConfigDataEx* FGsInvadeBattleLevelManager::GetInvadeWorldConfigDataEx()
{
	return UGsTableUtil::FindFirstRow<FGsSchemaInvadeWorldConfigDataEx>();
}
