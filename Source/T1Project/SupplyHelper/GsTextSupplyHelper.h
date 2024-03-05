#pragma once

#include "GsBaseSupplyHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCurrencySupplyInfoSet;
struct FGsSchemaItemSupplyInfoSet;
struct FGsSchemaSkillSupplyInfoSet;

class FGsTextSupplyHelper final : public FGsBaseSupplyHelper
{

public:
	FGsTextSupplyHelper();

public:
	FText GetTitleTextCurrencyType(const CurrencyType InCurrencyType) const;

	FText GetTitleTextItemId(const ItemId InItemId) const;

	FText GetTitleTextSkillId(const SkillId InUserSkillSetId) const;

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
};
