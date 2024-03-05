// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifySet;
struct FGsSchemaSkillNotifyElement;

/**
 *  FGsEffectTextParseSkillLevelDamage 와 FGsEffectTextParseSkillNotifyDamage 에서 같이 쓸 함수 모음
 * 
 * - 표기: 공격력 min~max% 피해 + 무속성 피해 value + XX속성 피해 value + 방어 무시 피해 value n회
 * - min, max가 같으면 min만 표시
 * - +피해들은 정보 있을 때만 표시
 * - CheckIsSameDamageAndAddCount 에서 같은데미지라고 판단되면, 
 *   FGsSchemaEffectTextData를 추가하지 않고 hit 증가, min/max 값만 갱신 함
 * 
 * - int[0] : executeConditionId(필수)
 * - int[1] : hitCount (필수)
 * - int[2] : minRate (필수)
 * - int[3] : maxRate (필수)
 *
 * IsAdditionalDamage가 true일 경우 아래를 저장함
 * - int[4] : 무속성 피해 값
 * - int[5] : 속성타입(DamageElementalType)
 * - int[6] : 속성 피해 값
 * - int[7] : 방어 무시 피해 값
 */
class FGsEffectTextFuncSkillDamage final
{
public:
	enum EGsETSkillDamageParam : uint8
	{
		ET_ExecuteConditionId = 0, // executeConditionId(필수)
		ET_HitCount = 1, // hitCount(필수)
		ET_MinRate = 2, // Damage MinRate(필수)
		ET_MaxRate = 3, // Damage MaxRate(필수)

		ET_AdditionalStart = 4, // 값이 있으면 저장하는 정보들

		ET_NoneDamage = ET_AdditionalStart, // 무속성 피해
		ET_ElementalType = 5, // 속성 타입(DamageElementalType)
		ET_ElementalValue = 6, // 속성 피해 값
		ET_IgnoreDefense = 7, // 방어 무시 피해 값
	};

public:
	FGsEffectTextFuncSkillDamage() = default;
	~FGsEffectTextFuncSkillDamage() {}

public:
 	static bool IsAdditionalDamage(const struct FGsSchemaSkillNotifyHitSet* InData);
	static bool SetData(EGsEffectTextParseType InParseType, const FGsSchemaSkillNotifySet* InData, 
		OUT TArray<FGsSchemaEffectTextData>& OutData);
private:
	static bool CheckIsSameDamageAndAddCount(EGsEffectTextParseType InParseType,
		int32 InExecuteConditionId, const FGsSchemaSkillNotifyHitSet* InHitSet,
		OUT TArray<FGsSchemaEffectTextData>& OutData);

public:
	static bool IsValidData(const FGsSchemaEffectTextData* InData);
	static void GetString(const FGsSchemaEffectTextData* InData, OUT FString& OutString);

private:
	static void GetStringDamage(const FGsSchemaEffectTextData* InData, OUT FString& OutString);

	static void AddStringHitCount(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
	static void AddStringNoElemental(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
	static void AddStringElemental(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
	static void AddStringIgnoreDefense(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
};
