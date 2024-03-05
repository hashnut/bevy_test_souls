#pragma once

#include "Classes/GsManager.h"
#include "Classes/ValidatorChain/GsCurrencyTypeAmountCachedValueMap.h"
#include "Classes/ValidatorChain/GsItemCategorySubAmountCachedValueMap.h"
#include "Classes/ValidatorChain/GsItemIdAmountCachedValueMap.h"
#include "Classes/ValidatorChain/GsItemIdLevelAmountCachedValueMap.h"
#include "Management/GsScopeHolder.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

enum class ECachedValueItemDelegateType
{
	DEFAULT,
	/** 즉시 처분 가능한 인벤토리 내 아이템 대상 */
	CONSUMABLE_INVENTORY = DEFAULT,
	/** 즉시 처분 가능한 인벤토리와 창고 내 아이템 대상 */
	CONSUMABLE_INVEN_DEPOT,
	/** 처분 가능 여부 무관하게 모든 인벤토리 내 아이템 대상 */
	ALL_INVENTORY,
	/** 처분 가능 여부 무관하게 모든 인벤토리와 창고 내 아이템 대상 */
	ALL_INVEN_DEPOT,
};

class FGsValidatorChainManager final : public IGsManager
{

private:
	TSharedPtr<FGsCurrencyTypeAmountCachedValueMap> _currencyTypeAmountCachedValueMaps;
	TMap<ECachedValueItemDelegateType, TSharedRef<FGsItemIdAmountCachedValueMap>> _itemIdAmountCachedValueMaps;
	TMap<ECachedValueItemDelegateType, TSharedRef<FGsItemIdLevelAmountCachedValueMap>> _itemIdLevelAmountCachedValueMaps;
	TSharedPtr<FGsItemCategorySubAmountCachedValueMap> _itemCategorySubAmountCachedValueMap;

public:
	void Initialize() final;
	void Finalize() final;

public:
	TSharedRef<FGsCurrencyTypeAmountCachedValue> GetCachedValue(const CurrencyType InCurrencyType);

	TSharedRef<FGsItemIdAmountCachedValue> GetCachedValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId);

	TSharedRef<FGsItemIdLevelAmountCachedValue> GetCachedValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId, const Level InItemLevel);

	TSharedRef<FGsItemCategorySubAmountCachedValue> GetCachedValue(const ItemCategorySub InItemCategorySub);

	Currency GetValue(const CurrencyType InCurrencyType);

	ItemAmount GetValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId);

	ItemAmount GetValue(const ECachedValueItemDelegateType InGetType, const ItemId InItemId, const Level InItemLevel);

	ItemAmount GetValue(const ItemCategorySub InItemCategorySub);

};

#define GSValidatorChain() UGsScopeHolder::GetGameManagerFType<FGsValidatorChainManager>(UGsScopeGame::EManagerType::ValidatorChain)
