#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "GsBaseDemandHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCurrencyDemandInfoSet;
struct FGsSchemaItemDemandInfoSet;

/**
 * 텍스트형 수요 헬퍼
 */
class FGsTextDemandHelper final : public FGsBaseDemandHelper
{

	friend class FGsUtilizingContentsManager;

private:
	using Super = FGsBaseDemandHelper;

public:
	FGsTextDemandHelper();

protected:
	FText GetTitleText(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const final;

	// 재화 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const final;
	bool TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage = nullptr) const final;

	// 아이템 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const final;
	bool TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage = nullptr) const final;

};
