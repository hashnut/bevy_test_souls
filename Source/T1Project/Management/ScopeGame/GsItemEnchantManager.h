// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Management/GsScopeHolder.h"

#include "GameObject/Define/GsGameObjectDefine.h"


class FGsItem;
class FGsItemIngredient;
class FGsItemEquipment;

struct FGsSchemaItemEquipEnchantRate;
struct GsSchemaItemEquipEnchantResult;
/**
 * 
 */
class T1PROJECT_API FGsItemEnchantManager final :
public IGsManager
{
public:
	const uint8 DISPLAY_STAT_SLOT_COUNT = 10;

	struct DisplayDefaultStatData
	{
	public:
		StatType _type = StatType::NONE;
		int32 _currentValue = 0;
		int32 _minExpectValue = 0;
		int32 _maxExpectValue = 0;
		bool _isCursedScroll = false;
	};

	struct DisplayPropertyStatData
	{
	public:
		bool _isCursed = false;
		bool _isLoseStat = false;
		StatType _type = StatType::NONE;
		int32 _currentValue = 0;
		int32 _currentLevel = 0;
		int32 _earnStatLevel = 0;
		int32 _deltaScrollValue = 0;
	};

	struct DisplayRateInfo
	{
	public:
		ItemEnchantResult _enchantType = ItemEnchantResult::NONE;
		Level _currentLevel = 0;
		Level _expectLevel = 0;
		float _rate = 0.f;

	public:
		DisplayRateInfo() = default;
		explicit DisplayRateInfo(ItemEnchantResult enchantType, Level currentLevel, Level expectLevel, float rate)
			:
			_enchantType(enchantType), _currentLevel(currentLevel), _expectLevel(expectLevel), _rate(rate)
		{
		}

	};


	enum EnchantEffectSoundResType : uint8
	{
		Sound_None = 0,
		SingleEnchant_Safe,
		SingleEnchant_Safe_Success,
		SingleEnchant_NotSafe_Touch,
		SingleEnchant_NotSafe_Wait,

		MultiEnchant_Start,
		MultiEnchant_End,

		Refine_Start,
		Refine_Success,

		Enchant_Process, // 별도 특수 상황에서 사용되는 사운드 (참강화)

		MagicalForge,

		Sound_Max,
	};

private:
	// <rateId , <level,tableData*>>
	TMap<int32, TMap<int32, const FGsSchemaItemEquipEnchantRate*>> _enchantRateDataMap;

	// ResultType , DeltaValue
	TMap<ItemEnchantResult, int32> _enchantResultDeltaValueMap;

private:
	TMap<EnchantEffectSoundResType, uint64> _enchantEffectSoundIdMap;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

private:
	void InitRateData();
	void InitEnchantResultDeltaData();

private:
	const ItemEnchantResult ConvertResultTypeByEnchantType(const ItemEnchantType InEnchantType);
	bool IsValidMultiEnchant(TWeakPtr<FGsItem> InValidCheckItemData);

public:
	void UpdateSingleEnchantItem(const ItemDataEquip* InTargetEquipItemData, const uint64 InScollItemDbid, const ItemEnchantResult InAckResult);
	void UpdateMultiEnchantItem(TArray<ItemEnchantResultData> InEnchantResultDataList, TArray<ItemDBId>& InUseIngredientItemDbidList);
	void UpdateRefineOptionItem(const ItemDataEquip* InTargetEquipItemData, const ItemRefineResult InAckResult);
	void UpdateMagicalForgeItem(const ItemDataEquip* InTargetEquipItemData, const ItemMagicalForgeResult InAckResult);

public:
	void AllStopSound();
	uint64 PlayEffectSound(EnchantEffectSoundResType InResType, const float InSpeed = 1.f);
	void StopEffectSound(EnchantEffectSoundResType InResType);
	FString GetSoundResName(EnchantEffectSoundResType InResType);

public:
	bool IsOpenItemEnchangWidget();
	bool IsOkInvenCondition();

public:
	bool PrepareMultiEnchantPacketData(TArray<ItemDBId> InEquipItemDbidList, TMap<ItemDBId, TPair<ItemId, ItemAmount>> InIngredientDataMap, bool InIsCursed, int32 InExpectLevel,
		OUT TArray<ItemDBId>& outEquipItemDbidList, OUT TArray<ItemDBIdAmountPair>& outIngredientItemDbidList, Level& outRealTargetLevel);
	bool IsContinueEnchant(TArray<ItemDBId> InEquipItemDbidList, bool InIsCursed, int32 InExpectLevel);
	bool IsMatchedEnchantItem(const ItemDBId InTargetItemDbid, const ItemDBId InComapreItemDbid, const bool InIsMultiEnchant);
	bool IsMatchedEnchantItem(EInventoryTab InCurInvenTabType, const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid, const ItemDBId InInvenItemDbid);
	bool IsValidSingleEnchant(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid, int32& InExpectLevel);
	bool IsValidRefine(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid);
	bool IsRiskEnchant(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid);
	uint8 IsRiskEnchant(TArray<ItemDBId> InEquipItemDbidList, const int32 InExpectLevel);
	bool IsRiskEnchantEffect(TWeakPtr<FGsItem> InTargetItemData, const ItemDBId InIngredientItemDbid);
	bool IsNotAllowedMultiEnchantItem(const ItemDBId InCheckItemDbid);

public:
	void FindMultiEnchantExpectNumState(TArray<ItemDBId> InItemDbidList, ItemId InIngredientItemTid, const uint8 InNumMaxCount ,OUT TArray<bool>& outStateList);
	ItemAmount GetMultiEnchantNeedScrollCount(TArray<ItemDBId> InItemDbidList, const bool InIsCursed, const int32 InExpectEnchantCount);
	ItemAmount GetMultiEnchantNeedScrollCount(TWeakPtr<FGsItem> InItemData, const bool InIsCursed, const int32 InExpectEnchantCount);
	float GetEnchantItemRate(const ItemEnchantResult InResultType, const int32 InRateId, const int32 InItemLevel);
	int32 GetOriEnchantItemRate(const ItemEnchantResult InResultType, const int32 InRateId, const int32 InItemLevel);
	TArray<float> GetDisplayEnchantItemRate(const int32 InRateId, const Level InTargetLevel, ItemEnchantType InEnchantType);
	int32 GetEnchantResultDeltaValue(FGsItemIngredient* InIngredientItem);
	Level GetEnchantLevel(const ItemDBId InTargetItemDbid);
	Currency GetEnchantCost(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid, const ItemAmount InNeedAmountCount);
	//void GetEnchantExpectRateInfo(TWeakPtr<FGsItem> InEquipItemData, TWeakPtr<FGsItem> InIngredientItemData, OUT TMap<TPair<Level, Level>, float>& outRateInfoMap);
	void GetEnchantExpectRateInfo(TWeakPtr<FGsItem> InEquipItemData, TWeakPtr<FGsItem> InIngredientItemData, OUT TArray<DisplayRateInfo>& outRateInfoList);

public:
	void GetEnchantDefaultStatInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, OUT TArray<DisplayDefaultStatData>& outStatDataList);
	bool GetEnchantPropertyStatInfo(FGsItemEquipment* InEquipData, TWeakPtr<FGsItem> InIngredientData, OUT TArray<DisplayPropertyStatData>& outStatDataList);
	bool GetEnchantSkillInfo(FGsItemEquipment* InEquipData, TWeakPtr<FGsItem> InIngredientData, OUT TOptional<TMap<Level, TArray<FText>>>& outSkillDataMap);
};

#define GSItemEnchant() UGsScopeHolder::GetGameManagerFType<FGsItemEnchantManager>(UGsScopeGame::EManagerType::ItemEnchant)