// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillLevelInfo;

/**
 * 스킬 기본/예외사항 혹은 effectText 있을 경우
 * - text[0] : effectText
 */
class T1PROJECT_API FGsEffectTextParseSkillLevelDefault final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillLevelDefault(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillLevelDefault() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_LEVEL_DEFAULT; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	
	virtual bool SetData(const FGsSchemaSkillLevelInfo* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
