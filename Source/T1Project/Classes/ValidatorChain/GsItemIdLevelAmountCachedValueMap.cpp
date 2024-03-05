#include "GsItemIdLevelAmountCachedValueMap.h"

#include "GsBaseCachedValue.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/************************************************************************/
/* FGsItemIdLevelAmountCollectorBase                                    */
/************************************************************************/

ItemAmount FGsItemIdLevelAmountCollectorBase::GetValue(const ItemId& InItemId, const Level& InItemMinLevel) const
{
	ItemAmount Amount = 0;

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
	}
	else
	{
		// TODO: need optimize
		const TArray<TWeakPtr<FGsItem>> Items = ItemManager->FindByTID(InItemId);
		for (TWeakPtr<FGsItem> Item : Items)
		{
			if (!Item.IsValid())
			{
				GSLOG(Error, TEXT("!Item.IsValid(), InItemId:%d"), InItemId);
				continue;
			}

			const FGsItem& CurrentItem = *Item.Pin().Get();
			if (!IsPassedItem(CurrentItem, InItemId, InItemMinLevel))
			{
				continue;
			}

			Amount += CurrentItem.GetAmount();
		}
	}

	return Amount;
}

void FGsItemIdLevelAmountCollectorBase::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		return;
	}

	TArray<TWeakPtr<FGsItem>> Items = ItemManager->FindByTID(InItemId);
	for (TWeakPtr<FGsItem>& Item : Items)
	{
		if (!Item.IsValid())
		{
			GSLOG(Error, TEXT("!Item.IsValid(), InItemId:%d"), InItemId);
			continue;
		}

		if (!IsPassedItem(*Item.Pin(), InItemId, InItemMinLevel))
		{
			continue;
		}

		OutItems.Emplace(Item);
	}
}

/************************************************************************/
/* FGsItemIdLevelAmountCollectorAllInventory                            */
/************************************************************************/

bool FGsItemIdLevelAmountCollectorAllInventory::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	if (InItem.GetTID() != InItemId)
	{
		return false;
	}

	// 창고 아이템 제외
	if (InItem.IsDepotItem())
	{
		return false;
	}

	if (InItem.GetLevel() != InItemMinLevel)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/* FGsItemIdLevelAmountCollectorAllInvenDepot                           */
/************************************************************************/

ItemAmount FGsItemIdLevelAmountCollectorAllInvenDepot::GetValue(const ItemId& InItemId, const Level& InItemMinLevel) const
{
	ItemAmount Amount = Super::GetValue(InItemId, InItemMinLevel);

	FGsDepotManager* DepotManager = GSDepot();
	if (nullptr == DepotManager)
	{
		return Amount;
	}

	TArray<TWeakPtr<FGsItem>> Items = DepotManager->FindByTID(InItemId);
	for (TWeakPtr<FGsItem>& Item : Items)
	{
		if (!Item.IsValid())
		{
			GSLOG(Error, TEXT("!Item.IsValid(), InItemId:%d"), InItemId);
			continue;
		}

		const FGsItem& CurrentItem = *Item.Pin();
		if (!IsPassedItem(CurrentItem, InItemId, InItemMinLevel))
		{
			continue;
		}

		Amount += CurrentItem.GetAmount();
	}

	return Amount;
}

void FGsItemIdLevelAmountCollectorAllInvenDepot::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	Super::AddAllPassedItems(OutItems, InItemId, InItemMinLevel);

	FGsDepotManager* DepotManager = GSDepot();
	if (nullptr == DepotManager)
	{
		return;
	}

	TArray<TWeakPtr<FGsItem>> Items = DepotManager->FindByTID(InItemId);
	for (TWeakPtr<FGsItem>& Item : Items)
	{
		if (!Item.IsValid())
		{
			GSLOG(Error, TEXT("!Item.IsValid(), InItemId:%d"), InItemId);
			continue;
		}

		if (!IsPassedItem(*Item.Pin(), InItemId, InItemMinLevel))
		{
			continue;
		}

		OutItems.Emplace(Item);
	}
}

bool FGsItemIdLevelAmountCollectorAllInvenDepot::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	if (InItem.GetTID() != InItemId)
	{
		return false;
	}

	// 잠김 장비 제외
	if (InItem.GetLock())
	{
		return false;
	}

	// 장착중 장비 제외
	if (InItem.IsEquipped())
	{
		return false;
	}

	// 프리셋 장착중인 장비 제외
	if (InItem.IsPresetEquipped())
	{
		return false;
	}

	// 다른 레벨이면 제외
	if (InItem.GetLevel() != InItemMinLevel)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/* FGsItemIdLevelAmountCollectorConsumableInventory                     */
/************************************************************************/

bool FGsItemIdLevelAmountCollectorConsumableInventory::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	if (InItem.GetTID() != InItemId)
	{
		return false;
	}

	// 창고 아이템 제외
	if (InItem.IsDepotItem())
	{
		return false;
	}

	// 잠김 장비 제외
	if (InItem.GetLock())
	{
		return false;
	}

	// 장착중 장비 제외
	if (InItem.IsEquipped())
	{
		return false;
	}

	// 프리셋 장착중인 장비 제외
	if (InItem.IsPresetEquipped())
	{
		return false;
	}

	if (InItem.GetLevel() != InItemMinLevel)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/* FGsItemIdLevelAmountCollectorConsumableInvenDepot                    */
/************************************************************************/

bool FGsItemIdLevelAmountCollectorConsumableInvenDepot::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	if (InItem.GetTID() != InItemId)
	{
		return false;
	}

	// 잠김 장비 제외
	if (InItem.GetLock())
	{
		return false;
	}

	// 장착중 장비 제외
	if (InItem.IsEquipped())
	{
		return false;
	}

	// 프리셋 장착중인 장비 제외
	if (InItem.IsPresetEquipped())
	{
		return false;
	}

	// 다른 레벨이면 제외
	if (InItem.GetLevel() != InItemMinLevel)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/* ItemId Level Amount Cached Value										*/
/************************************************************************/

FGsItemIdLevelAmountCachedValue::FGsItemIdLevelAmountCachedValue(const ItemId& InItemId, const Level& InItemMinLevel)
	: _itemId(InItemId)
	, _itemLevel(InItemMinLevel)
{
}

void FGsItemIdLevelAmountCachedValue::SetCollector(FGsItemIdLevelAmountCollectorBase& InCollector)
{
	_collector = &InCollector;
}

void FGsItemIdLevelAmountCachedValue::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	_collector->AddAllPassedItems(OutItems, _itemId, _itemLevel);
}

bool FGsItemIdLevelAmountCachedValue::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const
{
	return _collector->IsPassedItem(InItem, InItemId, InItemMinLevel);
}

void FGsItemIdLevelAmountCachedValue::UpdateValue()
{
	SetValue(_collector->GetValue(_itemId, _itemLevel));
}

/************************************************************************/
/* ItemId Level Amount Cached Value Map								    */
/************************************************************************/

FGsItemIdLevelAmountCachedValueMap::FGsItemIdLevelAmountCachedValueMap(TSharedRef<FGsItemIdLevelAmountCollectorBase> InCollector)
	: _collector(InCollector)
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsItemIdLevelAmountCachedValueMap::OnAddItemList));
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsItemIdLevelAmountCachedValueMap::OnUpdateItem));
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsItemIdLevelAmountCachedValueMap::OnRemoveItem));

	_itemEnchantActionTypeDelegates.Emplace(MessageHolder->GetItemEnchant().AddRaw(MessageContentEnchant::INVALIDATE, this, &FGsItemIdLevelAmountCachedValueMap::OnUpdateItemEnchant));
}

FGsItemIdLevelAmountCachedValueMap::~FGsItemIdLevelAmountCachedValueMap()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	for (const TPair<MessageItem, FDelegateHandle>& Delegate : _itemActionDelegates)
	{
		MessageHolder->GetItem().Remove(Delegate);
	}
	_itemActionDelegates.Empty();

	for (const TPair<MessageContentEnchant, FDelegateHandle>& Delegate : _itemEnchantActionTypeDelegates)
	{
		MessageHolder->GetItemEnchant().Remove(Delegate);
	}
	_itemEnchantActionTypeDelegates.Empty();
}

void FGsItemIdLevelAmountCachedValueMap::InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const
{
	StaticCast<FGsItemIdLevelAmountCachedValue*>(InCachedValue)->SetCollector(_collector.Get());
}

void FGsItemIdLevelAmountCachedValueMap::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	check(nullptr != Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	TMap<ItemId, TSet<Level>> ItemAndLevels;
	for (const ItemDBId CurrentItemDBId : Param->_itemDbidList)
	{
		TWeakPtr<FGsItem> ItemWeak = ItemManager->FindItem(CurrentItemDBId);
		if (false == ItemWeak.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, CurrentItemDBId: %lld"), CurrentItemDBId);
			continue;
		}

		TSharedPtr<FGsItem> Item = ItemWeak.Pin();
		ItemAndLevels.FindOrAdd(Item->GetTID()).Add(Item->GetLevel());
	}

	for (const TMap<ItemId, TSet<Level>>::ElementType& ItemIdAndLevel : ItemAndLevels)
	{
		for (const Level CurrentLevel : ItemIdAndLevel.Value)
		{
			this->SetDirty(ItemIdAndLevel.Key, CurrentLevel);
		}
	}
}

void FGsItemIdLevelAmountCachedValueMap::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(!!Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	TWeakPtr<FGsItem> Item = ItemManager->FindItem(Param->_itemDBID);
	if (false == Item.IsValid())
	{
		GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), Param->_itemDBID);
		return;
	}

	this->SetDirty(Item.Pin()->GetTID(), Item.Pin()->GetLevel());
}

void FGsItemIdLevelAmountCachedValueMap::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(!!Param);
	
	this->SetDirty(Param->_itemTID, Param->_itemLevel);
}

void FGsItemIdLevelAmountCachedValueMap::OnUpdateItemEnchant(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		GSLOG(Error, TEXT("nullptr == InParam"));
		return;
	}
	const FGsUpdateEnchantItem* UpdateEnchantItem = InParam->Cast<const FGsUpdateEnchantItem>();
	if (nullptr == UpdateEnchantItem)
	{
		GSLOG(Error, TEXT("nullptr == UpdateEnchantItem"));
		return;
	}

	if (INVALID_ITEM_DB_ID == UpdateEnchantItem->_equipItemDBID)
	{
		// 아이템 강화 후 아이템이 파괴되는 경우, INVALID_ITEM_DB_ID 일 수 있음.
		return;
	}

	TWeakPtr<FGsItem> EquipItem = GItem()->FindItem(UpdateEnchantItem->_equipItemDBID, ItemType::EQUIP);
	if (false == EquipItem.IsValid())
	{
		 GSLOG(Error, TEXT("nullptr == EquipItem, UpdateEnchantItem->_equipItemDBID:%llu"), UpdateEnchantItem->_equipItemDBID);
		return;
	}

	// 레벨이 변경된 경우만 SetDirty
	const Level CurrentLevel = EquipItem.Pin()->GetLevel();
	if (CurrentLevel != UpdateEnchantItem->_equipItemPrevLevel)
	{
		const ItemId EquipItemTid = EquipItem.Pin()->GetTID();
		this->SetDirty(EquipItemTid, UpdateEnchantItem->_equipItemPrevLevel); // 업데이트 전 레벨
		this->SetDirty(EquipItemTid, CurrentLevel); // 업데이트 후 레벨
	}
}
