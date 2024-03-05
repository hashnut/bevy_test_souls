// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillExecuteConditionSet;

/**
 * 스킬 발동 조건 Stat
 * "{Target}의 {StatName} {StatValue} {Condition} 일 때, 추가 효과:"
 * 예: 대상이 화상 보유시, 추가효과:
 *
 * - int[0] : SkillTargetType
 * - int[1] : StatType
 * - int[2] : SkillExecuteConditionStatConditionType
 * - int[3] : Value
 * - int[4] : SkillExecuteConditionStatConditionMethod 
 */
class T1PROJECT_API FGsEffectTextParseSkillExeConditionStat final : public FGsEffectTextParseBase
{
private:
	enum EGsETSkillConditionStatIndex : uint8
	{
		ET_TARGET = 0,
		ET_STAT_TYPE = 1,		
		ET_VALUE_TYPE = 2,
		ET_VALUE = 3,
		ET_CONDITION = 4,

		ET_MAX_INDEX,
	};

public:
	explicit FGsEffectTextParseSkillExeConditionStat(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillExeConditionStat() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_EXE_CONDITION_STAT; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	
	// 스킬의 조건 정보 세팅
	virtual bool SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

private:
	void GetTextInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText);

	void GetTextStatValue(int32 InValueType, int32 InValue, OUT FText& OutText);
	void GetTextCondition(int32 InType, OUT FText& OutText);
};
