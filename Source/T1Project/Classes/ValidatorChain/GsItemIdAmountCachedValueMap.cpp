#include "GsItemIdAmountCachedValueMap.h"

#include "GsBaseCachedValue.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Message/GsMessageItem.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/************************************************************************/
/* FGsItemIdAmountCollectorBase                                         */
/************************************************************************/

ItemAmount FGsItemIdAmountCollectorBase::GetValue(const ItemId& InItemId) const
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
			if (!IsPassedItem(CurrentItem, InItemId))
			{
				continue;
			}

			Amount += CurrentItem.GetAmount();
		}
	}

	return Amount;
}

void FGsItemIdAmountCollectorBase::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId) const
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

		if (!IsPassedItem(*Item.Pin(), InItemId))
		{
			continue;
		}

		OutItems.Emplace(Item);
	}
}

/************************************************************************/
/* FGsItemIdAmountCollectorAllInventory                                 */
/************************************************************************/

bool FGsItemIdAmountCollectorAllInventory::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const
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

	return true;
}

/************************************************************************/
/* FGsItemIdAmountCollectorAllInvenDepot                                */
/************************************************************************/

ItemAmount FGsItemIdAmountCollectorAllInvenDepot::GetValue(const ItemId& InItemId) const
{
	ItemAmount Amount = Super::GetValue(InItemId);

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
		if (!IsPassedItem(CurrentItem, InItemId))
		{
			continue;
		}

		Amount += CurrentItem.GetAmount();
	}

	return Amount;
}

void FGsItemIdAmountCollectorAllInvenDepot::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId) const
{
	Super::AddAllPassedItems(OutItems, InItemId);

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

		if (!IsPassedItem(*Item.Pin(), InItemId))
		{
			continue;
		}

		OutItems.Emplace(Item);
	}
}

bool FGsItemIdAmountCollectorAllInvenDepot::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const
{
	if (InItem.GetTID() != InItemId)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/* FGsItemIdAmountCollectorConsumableInventory                          */
/************************************************************************/

bool FGsItemIdAmountCollectorConsumableInventory::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const
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

	return true;
}

/************************************************************************/
/* FGsItemIdAmountCollectorConsumableInvenDepot                         */
/************************************************************************/

bool FGsItemIdAmountCollectorConsumableInvenDepot::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const
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

	return true;
}

/************************************************************************/
/* ItemId Amount Cached Value                                           */
/************************************************************************/

FGsItemIdAmountCachedValue::FGsItemIdAmountCachedValue(const ItemId& InItemId)
	: _itemId(InItemId)
{
}

void FGsItemIdAmountCachedValue::SetCollector(FGsItemIdAmountCollectorBase& InCollector)
{
	_collector = &InCollector;
}

void FGsItemIdAmountCachedValue::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	_collector->AddAllPassedItems(OutItems, _itemId);
}

bool FGsItemIdAmountCachedValue::IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const
{
	return _collector->IsPassedItem(InItem, InItemId);
}

void FGsItemIdAmountCachedValue::UpdateValue()
{
	SetValue(_collector->GetValue(_itemId));
}

/************************************************************************/
/* ItemId Amount Cached Value Map                                       */
/************************************************************************/

FGsItemIdAmountCachedValueMap::FGsItemIdAmountCachedValueMap(TSharedRef<FGsItemIdAmountCollectorBase> InCollector)
	: _collector(InCollector)
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsItemIdAmountCachedValueMap::OnAddItemList));
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsItemIdAmountCachedValueMap::OnUpdateItem));
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsItemIdAmountCachedValueMap::OnRemoveItem));
}

FGsItemIdAmountCachedValueMap::~FGsItemIdAmountCachedValueMap()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const TPair<MessageItem, FDelegateHandle>& Delegate : _listItemActionDelegates)
	{
		MessageManager->GetItem().Remove(Delegate);
	}
	_listItemActionDelegates.Empty();
}

void FGsItemIdAmountCachedValueMap::InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const
{
	StaticCast<FGsItemIdAmountCachedValue*>(InCachedValue)->SetCollector(_collector.Get());
}

void FGsItemIdAmountCachedValueMap::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	check(!!Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	TSet<ItemId> ItemIdSet;
	for (const ItemDBId CurrentItemDBId : Param->_itemDbidList)
	{
		TWeakPtr<FGsItem> Item = ItemManager->FindItem(CurrentItemDBId);
		if (false == Item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, CurrentItemDBId: %lld"), CurrentItemDBId);
			continue;
		}

		ItemIdSet.Add(Item.Pin()->GetTID());
	}

	for (const ItemId CurrentItemId : ItemIdSet)
	{
		this->SetDirty(CurrentItemId);
	}
}

void FGsItemIdAmountCachedValueMap::OnUpdateItem(FGsItemMessageParamBase& InParam)
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

	this->SetDirty(Item.Pin()->GetTID());
}

void FGsItemIdAmountCachedValueMap::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(!!Param);
	
	this->SetDirty(Param->_itemTID);
}
