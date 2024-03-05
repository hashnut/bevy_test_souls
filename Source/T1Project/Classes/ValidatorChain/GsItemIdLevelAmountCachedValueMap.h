#pragma once

#include "GsBaseCachedValue.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "TGsBaseCachedValueMap.h"

class FGsBaseCachedValue;
class FGsItem;
struct FGsEnchantMessageParamBase;
struct FGsItemMessageParamBase;
struct ItemDBIdAmountPair;

/**
 * FGsItemIdLevelAmountCollectorBase
 */
struct FGsItemIdLevelAmountCollectorBase
{
	virtual ItemAmount GetValue(const ItemId& InItemId, const Level& InItemMinLevel) const;

	virtual void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId, const Level& InItemMinLevel) const;

	virtual bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const = 0;
};

/**
 * FGsItemIdLevelAmountCollectorAllInventory
 */
struct FGsItemIdLevelAmountCollectorAllInventory final : public FGsItemIdLevelAmountCollectorBase
{
	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const final;
};

/**
 * FGsItemIdLevelAmountCollectorAllInvenDepot
 */
struct FGsItemIdLevelAmountCollectorAllInvenDepot : public FGsItemIdLevelAmountCollectorBase
{
private:
	using Super = FGsItemIdLevelAmountCollectorBase;

public:
	ItemAmount GetValue(const ItemId& InItemId, const Level& InItemMinLevel) const final;

	void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemId& InItemId, const Level& InItemMinLevel) const final;

	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const override;
};

/**
 * FGsItemIdLevelAmountCollectorConsumableInventory
 */
struct FGsItemIdLevelAmountCollectorConsumableInventory final : public FGsItemIdLevelAmountCollectorBase
{
public:
	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const final;
};

/**
 * FGsItemIdLevelAmountCollectorConsumableInvenDepot
 */
struct FGsItemIdLevelAmountCollectorConsumableInvenDepot final : public FGsItemIdLevelAmountCollectorAllInvenDepot
{
public:
	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const final;
};

/**
 * ItemId Level Amount Cached Value
 */
class FGsItemIdLevelAmountCachedValue final : public TGsBaseCachedValue<ItemAmount>
{
private:
	FGsItemIdLevelAmountCollectorBase* _collector;
	const ItemId _itemId;
	const Level _itemLevel;

public:
	FGsItemIdLevelAmountCachedValue(const ItemId& InItemId, const Level& InItemMinLevel);

	void SetCollector(FGsItemIdLevelAmountCollectorBase& InCollector);

	void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const;

	bool IsPassedItem(const FGsItem& InItem, const ItemId& InItemId, const Level& InItemMinLevel) const;

protected:
	void UpdateValue() final;
};

/**
 * ItemId Level Amount Cached Value Map
 */
class FGsItemIdLevelAmountCachedValueMap final : public TGsBaseCachedValueMap<FGsItemIdLevelAmountCachedValue, ItemId, Level>
{
private:
	TSharedRef<FGsItemIdLevelAmountCollectorBase> _collector;

	TArray<TPair<MessageItem, FDelegateHandle>>	_itemActionDelegates;

	TArray<TPair<MessageContentEnchant, FDelegateHandle>> _itemEnchantActionTypeDelegates;

public:
	FGsItemIdLevelAmountCachedValueMap(TSharedRef<FGsItemIdLevelAmountCollectorBase> InCollector);

	~FGsItemIdLevelAmountCachedValueMap() final;

protected:
	void InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const final;

private:
	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);

	void OnUpdateItemEnchant(const struct IGsMessageParam* InParam);
};
