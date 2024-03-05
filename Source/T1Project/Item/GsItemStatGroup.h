// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "Item/Data/GsItemEquipEnchantLoader.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Item/Data/GsItemStatData.h"
#include "Data/GsItemEquipEnchantLoader.h"

class FGsItemStat;
struct FGsSchemaPassivitySet;
struct FGsItemEquipEnchantBalance;
struct FGsItemEquipEnchantBonus;
struct FGsSchemaItemEquipRefineOption;
struct FGsSchemaItemEquipRefineOptionStatBalance;
/**
 * 
 */
class T1PROJECT_API FGsItemStatGroup
{

private:
	// 강화에 따른 스텟 정보
	// 현재 플레이어 스텟
	TArray<FGsItemStat*> _itemStatList;

	// BonusIndex : StatList / 특성스텟 데이터
	TMap<uint16 , FGsLevelBonusGroup> _itemBonusDataMap;

	// ItemLevel : StatList / Balance 스텟
	TMap<int32, TArray<FGsItemStat*>> _itemBalanceStatMap;

	// Skill Option
	// EnchantLevel , PassivityData
	TArray<TPair<uint8, const FGsSchemaPassivitySet*>> _itemSkillOptionList;

private:
	// 제련에 따른 스텟 정보(동적)
	TArray<FGsItemStat*> _itemRefineStatList;
	RefineBonusInfo* _itemRefineBonusInfo = nullptr;

private:
	const struct FGsItemEquipEnchantBonus* _equipBonusStatData = nullptr;
	const struct FGsSchemaItemEquipDetail* _equipDetailData = nullptr;
	TMap<int32, const FGsSchemaItemEquipRefineOption*> _equipRefineOptionDataMap;
	TMap<int32, const FGsSchemaItemEquipRefineOptionStatBalance*> _equipRefineOptionBalanceDataMap;

	// 마법 부여 정보
	MagicalForgeData _magicalForgeData;

public:
	FGsItemStatGroup(const FGsSchemaItemEquipDetail* InEquipDetailData);
	FGsItemStatGroup() = default;
	~FGsItemStatGroup();

private:
	void CreateItemStat(const StatType InType, const int32 InValue);
	void CreateItemBonusStatData();
	void CreateItemEnchantBalanceData(const FGsItemEquipEnchantBalance* InBalanceData);
	void CreateSkillOptionData();

private:
	const TArray<FGsItemStat*> FindBalanceDataByItemLevel(const int32 InItemLevel);

private:
	bool IsExistStat(StatType InType);
	bool IsExistDefaultStat(StatType InType);

private:
	void UpdateItemStat(const StatType InType, const int32 InValue);
	void RemoveItemStatList();

public:
	void UpdateEquipItemStat(const uint16 InItemLevel);
	//void UpdateRefineOptionStat(TArray<StatInfo*>& InStatInfoList, RefineBonusInfo InBonusInfo);
	void UpdateRefineOptionStat(const StatInfo InStatInfoArray[], RefineBonusInfo InBonusInfo);
	void UpdateRefineOptionStat(const TArray<FGsItemStat*>& InItemStatArray, RefineBonusInfo InBonusInfo);
	void UpdateMagicalForge(const MagicalForgeData& InMagicalForgeData);

public:
	const TMap<int32, TArray<FGsItemStatData>> GetBonusStatByIndex(const uint16 InIndex);
	const TArray<FGsItemStat*> GetItemStatWithBalanceValue(const int32 InItemLevel);
	const FString GetBonusStatTitleName(const uint16 InBonusIndex);
	const EGsItemEnchantBonusType GetBonusStatType(const uint16 InBonusIndex);
	const int32 GetOpenBonusStatLevelByStatType(const uint16 InBonusIndex, const StatType InStatType);
	const TArray<FGsItemStatData> GetEnchantExpectStatList(const int32 InExpectItemLevel);
	const TArray<FGsItemStat*> GetItemOriginalStatList();
	const TArray<FGsItemStat*>& GetEquipItemRefineStatList();
	TArray<TPair<uint8, const FGsSchemaPassivitySet*>> GetAllSkilOptionData();
	const bool GetEquipItemRefineBonusStatData(OUT TPair<StatType, int32>& outPairData);
	void GetAllExpectRefineOptionStatList(const ItemCategorySub InRefineOptionStoneType, TArray<int32> InRefineOptionID, OUT TMap<StatType, TPair<int32, int32>>& outDataMap);
	void GetAllExpectBonusRefineOptionStatList(TArray<int32> InBonusIdList, OUT TMap<StatType, TPair<int32, int32>>& outDataMap);
	const RefineBonusInfo* GetRefineBonusInfo();
	MagicalForgeData GetMagicalForgeData() { return _magicalForgeData; }
	bool IsExistEnchantLevel(int32 InLevel);


};
