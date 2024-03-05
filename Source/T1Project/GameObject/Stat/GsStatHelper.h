// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/stat/GsSchemaStatFormatingData.h"
#include "Stat/GsSchemaStatConvertTable.h"

struct FGsItemStatData;
class FGsItemStat;
class FGsStatInfo;
/**
 * 
 */
class T1PROJECT_API FGsStatHelper
{
public:
	FGsStatHelper() = default;
	~FGsStatHelper() = default;


private:
	static const FGsSchemaStatFormatingData* FindStatFormatingDataByType(const StatType InStatType);
	static bool GetFormatedValue(const FGsSchemaStatFormatingData* InData, const int32 InValue, OUT FString& outString, bool bUseSign = false);
	static bool GetConvertFormatedValue(const FGsSchemaStatFormatingData* InData, const int32 InValue, OUT FString& outString, bool bUseSign = false);
	static bool IsConvertRealDisplay(const FGsSchemaStatFormatingData* InData, const int32 InValue);

public:
	static bool GetDisPlayStatName(const StatType InStatType, OUT FText& outText);
	static bool GetStatIconImagePath(const StatType InStatType, OUT FString& outPathText);
	static bool GetDisPlayStatValueWithMarkFormat(const StatType InStatType, const int32 InValue, OUT FText& outText, bool bUseSign = false);
	static bool TryGetValidDecimalPointDigit(OUT int32& outDigit, float inValue, int32 inMaxDecimalPointDigit = 2);
	static FString GetRealNumberText(float inValue, bool inAddPercent = true);

	// 스탯명 +값 형태로 OutString에 저장. bUseSign: +부호를 붙이는지 여부.
	static bool GetDesplayStatNameAndValue(const StatType InStatType, const int32 InValue, 
		OUT FString& OutString, bool bUseSign = true);
	static bool GetDesplayStatNameAndValue(const StatType InStatType, const int32 InValue, 
		OUT TPair<FText, FText>& OutPair, bool bUseSign = true);	
	// 스탯명 +값(+추가값) 형태로 OutString에 저장. 예: 방어력 +1(+2).
	static bool GetStatWithAdditionalValue(const StatType InStatType, const int32 InValue, const int32 InAdditionalValue,
		OUT FString& OutString);
	// 스탯명 +값(+추가값) 형태로 OutString에 저장. 예: +1(+2).
	static bool GetStatNameAndValueWithAdditionalValue(const StatType InStatType, const int32 InValue, const int32 InAdditionalValue,
		OUT FText& OutTextName, OUT FString& OutStrValue);

	static void SortUiOrder(OUT TArray<FGsItemStatData>& outStatList);
	static void SortUiOrder(OUT TArray<FGsItemStat*>& outStatList);
	static void SortUiOrder(OUT TArray<const FGsStatInfo*>& outStatList);
	static void SortUiOrder(OUT TArray<StatInfo>& outStatList);

public:
	// 만분율 값을 실제 비율로 변환
	static float ConvertValueToPermyriad(const FGsStatInfo* inStatValue);
	// 속도(비율)를 처리하는 스텟에 하드캡을 적용한 값을 내보낸다.
	static float ApplyHardcapSpeedRateStatType(const StatType inStatType, float currentValue);

public:
	static bool TryGetConvertData(OUT FGsSchemaStatConvertTable& outData, StatType inStatType, int32 inValue);

	static int32 GetMaxBonusStatItemCount();
	static const FGsSchemaStatFormatingData* FindStat(StatType inStat);
	static const struct FGsSchemaCombatPowerData* FindCombatPowerStat(StatType inStat);
	static CreatureWeaponType GetPlayerWeapon();
};
