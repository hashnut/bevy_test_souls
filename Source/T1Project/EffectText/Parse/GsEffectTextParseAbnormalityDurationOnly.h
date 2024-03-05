// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaAbnormalitySet;
struct FGsSchemaEffectTextAbnormalityLevel;
struct FGsSchemaAbnormalityIndexSet;

/**
 * 이름과 효과 지속 시간을 찍는다. 지속시간이 같을 경우 min 값만 찍는다
 * - int[0] : AbnormalityEffectType
 * - float[0] : durationMin
 * - float[1] : durationMax 
 *
 * - 예시: 스턴 1초~2초
 * - 예시: 스턴 1초
 */
class T1PROJECT_API FGsEffectTextParseAbnormalityDurationOnly final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseAbnormalityDurationOnly(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParseAbnormalityDurationOnly() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::ABNORMALITY_DURATION_ONLY; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
		OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData) override;
};
