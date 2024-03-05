// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillLevelSet;
struct FGsSchemaSkillLevelInfo;
struct FGsSchemaSkillNotifySet;

/**
 * 스킬일 경우의 데미지 표시
 * - FGsEffectTextFuncSkillDamage의 주석 참고 
 */
class T1PROJECT_API FGsEffectTextParseSkillLevelDamage final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillLevelDamage(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillLevelDamage() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_LEVEL_DAMAGE; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	virtual bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData) override;
	
	// 주의: 모든 노티파이들의 데미지 정보를 보고 추가 혹은 합쳐 처리하고있음
	virtual bool SetData(const FGsSchemaSkillLevelInfo* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;	
	virtual bool SetData(const FGsSchemaSkillNotifySet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
