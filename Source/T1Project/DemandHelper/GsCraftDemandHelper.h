#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "GsBaseDemandHelper.h"
#include "GsCommonDemandHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaItemDemandInfoSet;
struct FGsSchemaCurrencyDemandInfoSet;

/**
 * 제작 수요 헬퍼
 */
class FGsCraftDemandHelper final : public FGsCommonDemandHelper
{

private:
	using Super = FGsCommonDemandHelper;

public:
	FGsCraftDemandHelper();

protected:
	// 재화 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const final;
	bool TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage = nullptr) const final;

	// 아이템 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const final;
	bool TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage = nullptr) const final;

private:
	static TArray<CraftId> MakeDisplayableCraftIds(const TSet<int32>& InCraftIds);

	static bool TryToNavigate(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId, const TSet<int32>& InCraftIds, OUT FText* OutFailMessage);

};
