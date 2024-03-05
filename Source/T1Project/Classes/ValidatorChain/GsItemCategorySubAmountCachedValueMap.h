#pragma once

#include "GsBaseCachedValue.h"
#include "Message/GsMessageItem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "TGsBaseCachedValueMap.h"

class FGsItem;
struct FGsItemMessageParamBase;

/**
 * FGsItemCategorySubAmountCollectorBase
 */
struct FGsItemCategorySubAmountCollectorBase
{
	virtual ItemAmount GetValue(const ItemCategorySub& InItemCategorySub) const;

	virtual void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems, const ItemCategorySub& InItemCategorySub) const;

	virtual bool IsPassedItem(const FGsItem& InItem, const ItemCategorySub& InItemCategorySub) const = 0;
};

/**
 * FGsItemCategorySubAmountCollectorConsumableInventory
 */
struct FGsItemCategorySubAmountCollectorConsumableInventory final : public FGsItemCategorySubAmountCollectorBase
{
	bool IsPassedItem(const FGsItem& InItem, const ItemCategorySub& InItemCategorySub) const final;
};

/**
 * ItemCategorySub Amount Cached Value
 */
class FGsItemCategorySubAmountCachedValue final : public TGsBaseCachedValue<ItemAmount>
{
private:
	FGsItemCategorySubAmountCollectorBase* _collector;
	const ItemCategorySub _itemCategorySub;

public:
	FGsItemCategorySubAmountCachedValue(const ItemCategorySub& InItemCategorySub);

	void SetCollector(FGsItemCategorySubAmountCollectorBase& InCollector);

	void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const;

	bool IsPassedItem(const FGsItem& InItem, const ItemCategorySub& InItemCategorySub) const;

protected:
	void UpdateValue() final;
};

/**
 * ItemCategorySub Amount Cached Value Map
 */
class FGsItemCategorySubAmountCachedValueMap final : public TGsBaseCachedValueMap<FGsItemCategorySubAmountCachedValue, ItemCategorySub>
{
private:
	TSharedRef<FGsItemCategorySubAmountCollectorBase> _collector;

	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;

public:
	FGsItemCategorySubAmountCachedValueMap(TSharedRef<FGsItemCategorySubAmountCollectorBase> InCollector);

	~FGsItemCategorySubAmountCachedValueMap() final;

protected:
	void InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const final;

private:
	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);
};
