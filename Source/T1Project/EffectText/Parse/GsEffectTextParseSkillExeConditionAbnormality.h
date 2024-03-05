// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillExecuteConditionSet;

/**
 * 스킬 발동 조건 Abnormality
 * "{Target}|hpp(이,가) {Name}을 {Condition}, 추가 효과:"
 * 예: 대상이 화상 보유시, 추가효과:
 * 
 * - int[0] : SkillTargetType
 * - int[1] : SkillExecuteConditionAbnormalityConditionType
 * - int[2] : AbnoramlityId 혹은 AbnormalityEffectType 
 * - int[3] : SetAbnormalityAllow
 */
class T1PROJECT_API FGsEffectTextParseSkillExeConditionAbnormality final : public FGsEffectTextParseBase
{
private:
	enum EGsETSkillConditionAbIndex : uint8
	{
		ET_TARGET = 0,
		ET_ABNORMALITY_DATA_TYPE = 1,
		ET_ABNORMALITY_DATA_VALUE = 2,
		ET_ALLOW_TYPE = 3,

		ET_MAX_INDEX,
	};

public:
	explicit FGsEffectTextParseSkillExeConditionAbnormality(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillExeConditionAbnormality() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_EXE_CONDITION_ABNORMALITY; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	
	// 스킬의 조건 정보 세팅
	virtual bool SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

private:
	void GetTextInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText);

	void GetTextAbnormalityName(int32 InType, int32 InValue, OUT FText& OutText);
	void GetTextAllowType(int32 InType, OUT FText& OutText);
};
