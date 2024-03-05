// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaAbnormalitySet;
struct FGsSchemaAbnormalityStatSet;
struct FGsSchemaAbnormalityStat;
struct FGsSchemaEffectTextAbnormalityLevel;
struct FGsSchemaAbnormalityIndexSet;

/**
 * FGsStatHelper를 이용해 스탯을 찍는다.
 * - int[0] : StatType
 * - int[1] : 값
 * - float[0] : durationMin
 * - float[1] : durationMax
 *
 * - 예: 방어력 +10 1~2초
 * - 예: 회피율 +10% 2초
 */
class T1PROJECT_API FGsEffectTextParseAbnormalityStat final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseAbnormalityStat(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParseAbnormalityStat() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::ABNORMALITY_STAT; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, OUT int32 InOption, TArray<TPair<FText, FText>>& OutData) override;
	virtual bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData) override;

	virtual bool GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) override;
	
	virtual bool SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
		OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData) override;

private:
	void GetTimeString(float InTimeMin, float InTimeMax, OUT FString& OutStr);
};
