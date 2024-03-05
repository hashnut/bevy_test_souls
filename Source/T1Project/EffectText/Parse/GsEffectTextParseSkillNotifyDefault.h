// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifySet;
struct FGsSchemaSkillNotifyElement;

/**
 * 노티파이 기본.
 * - text[0] : effectText. 이 정보 있으면 다른 것 무시하고 무조건 이걸 찍음
 */
class T1PROJECT_API FGsEffectTextParseSkillNotifyDefault final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillNotifyDefault(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillNotifyDefault() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_NOTIFY_DEFAULT; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetData(const FGsSchemaSkillNotifySet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
	virtual bool SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
