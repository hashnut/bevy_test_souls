#include "GsValidatorChainManager.h"

#include "Runtime/Core/Public/Templates/Tuple.h"

#include "Classes/ValidatorChain/GsCurrencyTypeAmountCachedValueMap.h"
#include "Classes/ValidatorChain/GsItemIdAmountCachedValueMap.h"
#include "Classes/ValidatorChain/GsItemIdLevelAmountCachedValueMap.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

void FGsValidatorChainManager::Initialize()
{
	_currencyTypeAmountCachedValueMaps = MakeShared<FGsCurrencyTypeAmountCachedValueMap>(MakeShared<FGsCurrencyTypeAmountCollector>());

	_itemIdAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::CONSUMABLE_INVENTORY, MakeShared<FGsItemIdAmountCachedValueMap>(MakeShared<FGsItemIdAmountCollectorConsumableInventory>()));
	_itemIdAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT, MakeShared<FGsItemIdAmountCachedValueMap>(MakeShared<FGsItemIdAmountCollectorConsumableInvenDepot>()));
	_itemIdAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::ALL_INVENTORY, MakeShared<FGsItemIdAmountCachedValueMap>(MakeShared<FGsItemIdAmountCollectorAllInventory>()));
	_itemIdAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::ALL_INVEN_DEPOT, MakeShared<FGsItemIdAmountCachedValueMap>(MakeShared<FGsItemIdAmountCollectorAllInvenDepot>()));

	_itemIdLevelAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::CONSUMABLE_INVENTORY, MakeShared<FGsItemIdLevelAmountCachedValueMap>(MakeShared<FGsItemIdLevelAmountCollectorConsumableInventory>()));
	_itemIdLevelAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT, MakeShared<FGsItemIdLevelAmountCachedValueMap>(MakeShared<FGsItemIdLevelAmountCollectorConsumableInvenDepot>()));
	_itemIdLevelAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::ALL_INVENTORY, MakeShared<FGsItemIdLevelAmountCachedValueMap>(MakeShared<FGsItemIdLevelAmountCollectorAllInventory>()));
	_itemIdLevelAmountCachedValueMaps.Emplace(ECachedValueItemDelegateType::ALL_INVEN_DEPOT, MakeShared<FGsItemIdLevelAmountCachedValueMap>(MakeShared<FGsItemIdLevelAmountCollectorAllInvenDepot>()));

	_itemCategorySubAmountCachedValueMap = MakeShared<FGsItemCategorySubAmountCachedValueMap>(MakeShared<FGsItemCategorySubAmountCollectorConsumableInventory>());
}

void FGsValidatorChainManager::Finalize()
{
}

TSharedRef<FGsCurrencyTypeAmountCachedValue> FGsValidatorChainManager::GetCachedValue(const CurrencyType InCurrencyType)
{
	return _currencyTypeAmountCachedValueMaps->GetCachedValue(InCurrencyType);
}

TSharedRef<FGsItemIdAmountCachedValue> FGsValidatorChainManager::GetCachedValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId)
{
	return _itemIdAmountCachedValueMaps[InGetType]->GetCachedValue(InItemId);
}

TSharedRef<FGsItemIdLevelAmountCachedValue> FGsValidatorChainManager::GetCachedValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId, const Level InItemLevel)
{
	return _itemIdLevelAmountCachedValueMaps[InGetType]->GetCachedValue(InItemId, InItemLevel);
}

TSharedRef<FGsItemCategorySubAmountCachedValue> FGsValidatorChainManager::GetCachedValue(const ItemCategorySub InItemCategorySub)
{
	return _itemCategorySubAmountCachedValueMap->GetCachedValue(InItemCategorySub);
}

Currency FGsValidatorChainManager::GetValue(const CurrencyType InCurrencyType)
{
	return _currencyTypeAmountCachedValueMaps->GetValue(InCurrencyType);
}

ItemAmount FGsValidatorChainManager::GetValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId)
{
	return _itemIdAmountCachedValueMaps[InGetType]->GetValue(InItemId);
}

ItemAmount FGsValidatorChainManager::GetValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId, const Level InItemLevel)
{
	return _itemIdLevelAmountCachedValueMaps[InGetType]->GetValue(InItemId, InItemLevel);
}

ItemAmount FGsValidatorChainManager::GetValue(const ItemCategorySub InItemCategorySub)
{
	return _itemCategorySubAmountCachedValueMap->GetValue(InItemCategorySub);
}
