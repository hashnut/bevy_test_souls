// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillExecuteConditionSet;

/**
 * 스킬 발동 조건 Passivity
 * {Target}|hpp(이,가) {Name}을 가지고 있을 때, 추가 효과:"
 * 예: 대상이 화상 보유시, 추가효과:
 * - int[0] : SkillTargetType
 * - int[1] : PassivityId 
 */
class T1PROJECT_API FGsEffectTextParseSkillExeConditionPassivity final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillExeConditionPassivity(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillExeConditionPassivity() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_EXE_CONDITION_PASSIVITY; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	
	// 스킬의 조건 정보 세팅
	virtual bool SetSkillConditionData(const FGsSchemaSkillExecuteConditionSet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

private:
	void GetTextInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText);
};
