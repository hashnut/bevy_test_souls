#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Unlock/GsUnlockDefine.h"

struct FGsSchemaCurrencyDemandInfoSet;
struct FGsSchemaItemDemandInfoSet;

/**
 * 베이스 수요 헬퍼
 */
class FGsBaseDemandHelper
{

	friend class FGsUtilizingContentsManager;

public:
	const EGsUnlockContentsType _contentsType;

public:
	FGsBaseDemandHelper(const EGsUnlockContentsType InContentsType);

	FGsBaseDemandHelper();

	virtual ~FGsBaseDemandHelper();

protected:
	virtual FText GetTitleText(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const;

	// 재화 수요처 세트 헬퍼
	virtual UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const;
	virtual bool TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage = nullptr) const;

	// 아이템 수요처 세트 헬퍼
	virtual UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const;
	virtual bool TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage = nullptr) const;

protected:
	// Extra 로 상태 반환
	static UGsSupplyManager::EGsNavigationStatus GetNavigationStatusStatic(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType);

	static UGsSupplyManager::EGsNavigationStatus GetNavigationStatusStatic(const EGsUnlockContentsType InContentsType, const ItemId InItemId);

//private:
//	static UGsSupplyManager::EGsNavigationStatus GetNavigationStatusStatic(const EGsUnlockContentsType InContentsType, const FGsSchemaDemandInfoConstraint& InDemandInfoConstraint);

#if WITH_EDITOR
	// 데이터테이블 자동 생성 시 활용 가능한 함수를 이 곳에서 정의하는 것도 고려중.
#endif // WITH_EDITOR

};
