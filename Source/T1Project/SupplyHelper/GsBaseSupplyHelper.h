#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Unlock/GsUnlockDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"

/**
 * 베이스 공급 헬퍼
 */
class FGsBaseSupplyHelper
{

	friend class UGsSupplyManager;

public:
	const EGsUnlockContentsType _contentsType;

public:
	FGsBaseSupplyHelper(const EGsUnlockContentsType InContentsType)
		: _contentsType(InContentsType)
	{
	}

	virtual ~FGsBaseSupplyHelper()
	{
	}

protected:
	virtual FText GetTitleText() const = 0;

	// 재화 공급처 세트 헬퍼
	virtual UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet) const = 0;

	virtual bool TryToNavigate(const FGsSchemaCurrencySupplyInfoSet& InCurrencySupplyInfoSet, OUT FText* OutFailMessage = nullptr) const = 0;

	// 아이템 공급처 세트 헬퍼
	virtual UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet) const = 0;

	virtual bool TryToNavigate(const FGsSchemaItemSupplyInfoSet& InItemSupplyInfoSet, OUT FText* OutFailMessage = nullptr) const = 0;

	// 멀티플 아이템 공급처 세트 헬퍼
	virtual UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet) const = 0;

	virtual bool TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSets, OUT FText* OutFailMessage = nullptr) const = 0;
	
	// 개별 공급처 헬퍼
	//virtual bool TryToNavigate(const FGsSchemaSupplyInfo& InSupplyInfo) const = 0;

#if WITH_EDITOR
	// 데이터테이블 자동 생성 시 활용 가능한 함수를 이 곳에서 정의하는 것도 고려중.
#endif // WITH_EDITOR

};
