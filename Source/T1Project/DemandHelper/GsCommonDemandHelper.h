#pragma once

#include "GsBaseDemandHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Unlock/GsUnlockDefine.h"

struct FGsSchemaCurrencyDemandInfoSet;
struct FGsSchemaItemDemandInfoSet;

/**
 * 공용 수요 헬퍼
 */
class FGsCommonDemandHelper : public FGsBaseDemandHelper
{

private:
	using Super = FGsBaseDemandHelper;

public:
	FGsCommonDemandHelper(const EGsUnlockContentsType InContentsType);

protected:
	// 재화 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const override;
	bool TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage = nullptr) const override;

	// 아이템 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const override;
	bool TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage = nullptr) const override;

private:
	static bool TryToNavigate(const EGsUnlockContentsType InContentsType, OUT FText* OutFailMessage);

};
