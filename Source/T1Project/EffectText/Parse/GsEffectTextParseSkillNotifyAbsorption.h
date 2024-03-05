// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifyElement;

/**
 * 노티파이의 hpAbsorptionRate 정보
 * - hpAbsorptionRate 표기: 피해량의 n% HP로 흡수  // hpAbsorptionRate 의 경우
 * - hpAbsorption 표기: HP n 흡수
 * - text[0] : effectText. 이 정보 있으면 다른 것 무시하고 무조건 이걸 찍음
 * - int[0] : hpAbsorptionRate
 * - int[1] : hpAbsorption
 */
class T1PROJECT_API FGsEffectTextParseSkillNotifyAbsorption final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillNotifyAbsorption(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillNotifyAbsorption() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_NOTIFY_ABSORPTION; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

private:
	bool GetStringAbsorption(int32 InRate, int32 InValue, OUT FString& OutString);
};
