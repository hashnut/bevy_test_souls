#pragma once

#include "GsBaseSupplyHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCurrencySupplyInfoSet;
struct FGsSchemaItemSupplyInfoSet;
struct FGsSchemaSupplyInfoConstraint;
struct FGsSchemaSkillSupplyInfoSet;

class FGsBMShopSupplyHelper final : public FGsBaseSupplyHelper
{

public:
	FGsBMShopSupplyHelper();

public:
	FText GetTitleText() const final;

	// 재화 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet) const final;
	bool TryToNavigate(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage = nullptr) const final;

	// 아이템 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet) const final;
	bool TryToNavigate(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage = nullptr) const final;

	// 멀티플 아이템 공급처 세트 헬퍼
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet) const final;
	bool TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage = nullptr) const final;

private:
	static bool CanDisplay(const int32& InBmshopTabId);

	static bool TryToNavigate(const int32& InBmshopTabId, OUT FText* OutFailMessage);

	static bool GetBMShopTabIdCurrencyType(const CurrencyType InCurrencyType, OUT int32& OutBMShopTabId);

	static bool GetBMShopTabIdItemId(const ItemId InItemId, OUT int32& OutBMShopTabId);

	/** 스킬북 획득처(상점)의 경우, 하나 이상일 수 있어, 접근 가능한 첫번째 상점을 반환함. */
	static bool GetAllBMShopTabIdsBySkillId(const SkillId InSkillId, OUT TArray<int32>& OutBMShopTabIds);

	static bool GetBMShopTabIdSupplyInfoConstraint(const FGsSchemaSupplyInfoConstraint* InSupplyInfoSet, OUT int32& OutBMShopTabId);

};
