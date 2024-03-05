#pragma once

#include "GsBaseSupplyHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCraftSupplyInfo;
struct FGsSchemaCurrencySupplyInfoSet;
struct FGsSchemaItemSupplyInfoSet;
struct FGsSchemaSkillSupplyInfoSet;
struct FGsSchemaSkillSupplyInfoSet;

class FGsCraftSupplyHelper final : public FGsBaseSupplyHelper
{

public:
	FGsCraftSupplyHelper();

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
	bool TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSets, OUT FText* OutFailMessage = nullptr) const final;

private:
	/**
	 * CraftId 중에서 현재 가장 유용한 제작ID 를 반환.
	 */
	static CraftId FindBestCraftId(const TSet<int32>& InCraftIds);

	static UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const TSet<int32>& InCraftIds);

	static bool TryToNavigate(const TSet<int32>& InCraftIds, OUT FText* OutFailMessage = nullptr);
};
