#pragma once

#include "GsBaseSupplyHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCurrencySupplyInfoSet;
struct FGsSchemaDropSupplyInfo;
struct FGsSchemaDropSupplyInfoCreatureIds;
struct FGsSchemaItemSupplyInfoSet;
struct FGsSchemaWorldMapMonsterElementDataRow;
struct FGsSchemaSkillSupplyInfoSet;
struct FGsSchemaSkillSupplyInfoSet;

class FGsDropSupplyHelper final : public FGsBaseSupplyHelper
{

public:
	FGsDropSupplyHelper();

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
	static bool CanDisplay(const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>& InMapCreatureMap, const TMap<int32, FGsSchemaDropSupplyInfo>& InContentsEventMapCreatureSetMap);

	static bool TryToNavigateItemId(const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>& InMapCreatureMap, const TMap<int32, FGsSchemaDropSupplyInfo>& InContentsEventMapCreatureSetMap, const ItemId& InItemId, OUT FText* OutFailMessage);

	static void MakeNavigationList(OUT TArray<FGsSchemaWorldMapMonsterElementDataRow>& OutNavigationList, const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>* InMapCreatureMap, const TMap<int32, FGsSchemaDropSupplyInfo>* InContentsEventMapCreatureSetMap);

	static bool FindDropSupplyInfos(const ItemId InItemId, OUT const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>*& OutMaps, OUT const TMap<int32, FGsSchemaDropSupplyInfo>*& OutContentsEventMaps);

public:
	static bool MakeNavigationList(OUT TArray<FGsSchemaWorldMapMonsterElementDataRow>& OutNavigationList, const ItemId InItemId);

};
