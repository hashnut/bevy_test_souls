#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "GsCommonDemandHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCurrencyDemandInfoSet;
struct FGsSchemaDemandInfoConstraint;
struct FGsSchemaItemDemandInfoSet;

/**
 * BM상점 수요 헬퍼
 */
class FGsBMShopDemandHelper final : public FGsCommonDemandHelper
{

private:
	using Super = FGsCommonDemandHelper;

public:
	FGsBMShopDemandHelper();

protected:
	// 재화 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const final;
	bool TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InDemandInfoSet, OUT FText* OutFailMessage = nullptr) const final;

	// 아이템 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const final;
	bool TryToNavigate(const FGsSchemaItemDemandInfoSet& InDemandInfoSet, OUT FText* OutFailMessage = nullptr) const final;

private:
	static bool CanDisplay(const int32& InBmshopTabId);

	static bool TryToNavigate(const int32& InBmshopTabId, OUT FText* OutFailMessage);

	static bool GetBMShopTabId(const CurrencyType InCurrencyType, OUT int32& OutBMShopTabId);

	static bool GetBMShopTabId(const ItemId InItemId, OUT int32& OutBMShopTabId);

	static bool GetBMShopTabId(const FGsSchemaDemandInfoConstraint* InDemandInfoSet, OUT int32& OutBMShopTabId);

};
