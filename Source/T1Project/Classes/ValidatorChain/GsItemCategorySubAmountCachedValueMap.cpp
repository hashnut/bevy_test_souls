#include "GsItemCategorySubAmountCachedValueMap.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
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
/* FGsItemCategorySubAmountCollectorBase                                */
/************************************************************************/

ItemAmount FGsItemCategorySubAmountCollectorBase::GetValue(const ItemCategorySub& InItemCategorySub) const
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
		const TArray<TWeakPtr<FGsItem>> Items = ItemManager->FindItem(InItemCategorySub);
		for (const TWeakPtr<FGsItem>& Item : Items)
		{
			if (!Item.IsValid())
			{
				GSLOG(Error, TEXT("!Item.IsValid(), InItemCategorySub:%d"), InItemCategorySub);
				continue;
			}

			const FGsItem& CurrentItem = *Item.Pin().Get();
			if (!IsPassedItem(CurrentItem, InItemCategorySub))
			{
				continue;
			}

			Amount += CurrentItem.GetAmount();
		}
	}

	return Amount;
}

void FGsItemCategorySubAmountCollectorBase::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemCategorySub& InItemCategorySub) const
{
	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		return;
	}

	TArray<TWeakPtr<FGsItem>> Items = ItemManager->FindItem(InItemCategorySub);
	for (TWeakPtr<FGsItem>& Item : Items)
	{
		if (!Item.IsValid())
		{
			GSLOG(Error, TEXT("!Item.IsValid(), InItemCategorySub:%d"), InItemCategorySub);
			continue;
		}

		if (!IsPassedItem(*Item.Pin(), InItemCategorySub))
		{
			continue;
		}

		OutItems.Emplace(Item);
	}
}

/************************************************************************/
/* FGsItemCategorySubAmountCollectorConsumableInventory                 */
/************************************************************************/

bool FGsItemCategorySubAmountCollectorConsumableInventory::IsPassedItem(const FGsItem& InItem, const ItemCategorySub& InItemCategorySub) const
{
	if (InItem.GetSubCategory() != InItemCategorySub)
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
/* ItemCategorySub Amount Cached Value                                  */
/************************************************************************/

FGsItemCategorySubAmountCachedValue::FGsItemCategorySubAmountCachedValue(const ItemCategorySub& InItemCategorySub)
	: _itemCategorySub(InItemCategorySub)
{
}

void FGsItemCategorySubAmountCachedValue::SetCollector(FGsItemCategorySubAmountCollectorBase& InCollector)
{
	_collector = &InCollector;
}

void FGsItemCategorySubAmountCachedValue::AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const
{
	_collector->AddAllPassedItems(OutItems, _itemCategorySub);
}

bool FGsItemCategorySubAmountCachedValue::IsPassedItem(const FGsItem& InItem, const ItemCategorySub& InItemCategorySub) const
{
	return _collector->IsPassedItem(InItem, InItemCategorySub);
}

void FGsItemCategorySubAmountCachedValue::UpdateValue()
{
	SetValue(_collector->GetValue(_itemCategorySub));
}

/************************************************************************/
/* ItemCategorySub Amount Cached Value Map                              */
/************************************************************************/

FGsItemCategorySubAmountCachedValueMap::FGsItemCategorySubAmountCachedValueMap(TSharedRef<FGsItemCategorySubAmountCollectorBase> InCollector)
	: _collector(InCollector)
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsItemCategorySubAmountCachedValueMap::OnAddItemList));
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsItemCategorySubAmountCachedValueMap::OnUpdateItem));
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsItemCategorySubAmountCachedValueMap::OnRemoveItem));
}

FGsItemCategorySubAmountCachedValueMap::~FGsItemCategorySubAmountCachedValueMap()
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

void FGsItemCategorySubAmountCachedValueMap::InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const
{
	StaticCast<FGsItemCategorySubAmountCachedValue*>(InCachedValue)->SetCollector(_collector.Get());
}

void FGsItemCategorySubAmountCachedValueMap::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	check(!!Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	TSet<ItemCategorySub> ItemCategorySubSet;
	for (const ItemDBId CurrentItemDBId : Param->_itemDbidList)
	{
		TWeakPtr<FGsItem> Item = ItemManager->FindItem(CurrentItemDBId);
		if (false == Item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, CurrentItemDBId: %lld"), CurrentItemDBId);
			continue;
		}

		ItemCategorySubSet.Add(Item.Pin()->GetSubCategory());
	}

	for (const ItemCategorySub CurrentItemCategorySub : ItemCategorySubSet)
	{
		this->SetDirty(CurrentItemCategorySub);
	}
}

void FGsItemCategorySubAmountCachedValueMap::OnUpdateItem(FGsItemMessageParamBase& InParam)
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

	this->SetDirty(Item.Pin()->GetSubCategory());
}

void FGsItemCategorySubAmountCachedValueMap::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(!!Param);

	if (INVALID_ITEM_ID == Param->_itemTID)
	{
		GSLOG(Warning, TEXT("INVALID_ITEM_ID == Param->_itemTID"));
		return;
	}

	const UGsTableItemCommon* ItemCommonTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == ItemCommonTable)
	{
		GSLOG(Warning, TEXT("nullptr == ItemCommonTable"));
		return;
	}

	const FGsSchemaItemCommon* ItemCommon = nullptr;
	if (!ItemCommonTable->FindRowById(Param->_itemTID, ItemCommon))
	{
		GSLOG(Warning, TEXT("ItemCommonTable->FindRowById(Param->_itemTID, ItemCommon), Param->_itemTID:%d"), Param->_itemTID);
		return;
	}
		
	this->SetDirty(ItemCommon->categorySub);
}
