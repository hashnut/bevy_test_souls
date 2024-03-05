#pragma once

#include "GsBaseCachedValue.h"
#include "Message/GsMessageItem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "TGsBaseCachedValueMap.h"

class FGsItem;
struct FGsItemMessageParamBase;

/**
 * FGsItemIdAmountCollectorBase
 */
struct FGsItemIdAmountCollectorBase
{
	virtual ItemAmount GetValue(const ItemId& InItemId) const;

	virtual void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId) const;

	virtual bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const = 0;
};

/**
 * FGsItemIdAmountCollectorAllInventory
 */
struct FGsItemIdAmountCollectorAllInventory final : public FGsItemIdAmountCollectorBase
{
public:
	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const final;
};

/**
 * FGsItemIdAmountCollectorAllInvenDepot
 */
struct FGsItemIdAmountCollectorAllInvenDepot : public FGsItemIdAmountCollectorBase
{
private:
	using Super = FGsItemIdAmountCollectorBase;

public:
	ItemAmount GetValue(const ItemId& InItemId) const final;

	void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId) const final;

	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const override;
};

/**
 * FGsItemIdAmountCollectorConsumableInventory
 */
struct FGsItemIdAmountCollectorConsumableInventory final : public FGsItemIdAmountCollectorBase
{
	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const final;
};

/**
 * FGsItemIdAmountCollectorConsumableNowWithDepot
 */
struct FGsItemIdAmountCollectorConsumableInvenDepot final : public FGsItemIdAmountCollectorAllInvenDepot
{
public:
	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const final;
};

/**
 * ItemId Amount Cached Value
 */
class FGsItemIdAmountCachedValue final : public TGsBaseCachedValue<ItemAmount>
{
private:
	FGsItemIdAmountCollectorBase* _collector;
	const ItemId _itemId;

public:
	FGsItemIdAmountCachedValue(const ItemId& InItemId);

	void SetCollector(FGsItemIdAmountCollectorBase& InCollector);

	void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const;

	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId) const;

protected:
	void UpdateValue() final;
};

/**
 * ItemId Amount Cached Value Map
 */
class FGsItemIdAmountCachedValueMap final : public TGsBaseCachedValueMap<FGsItemIdAmountCachedValue, ItemId>
{
private:
	TSharedRef<FGsItemIdAmountCollectorBase> _collector;

	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;

public:
	FGsItemIdAmountCachedValueMap(TSharedRef<FGsItemIdAmountCollectorBase> InCollector);

	~FGsItemIdAmountCachedValueMap() final;

protected:
	void InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const final;

private:
	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);
};
