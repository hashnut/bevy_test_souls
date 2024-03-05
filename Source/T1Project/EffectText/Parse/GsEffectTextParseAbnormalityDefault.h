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
 * 이름만 찍는다
 * - text[0] : 이름 or effectText(있을 경우만)
 *
 * // 이 시간은 별도로 시간 찍을때만 가져감
 * - int[0] : AbnormalityEffectType
 * - float[0] : durationMin
 * - float[1] : durationMax 
 * 
 * - 예: 워프불가
 */
class T1PROJECT_API FGsEffectTextParseAbnormalityDefault final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseAbnormalityDefault(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParseAbnormalityDefault() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::ABNORMALITY_DEFAULT; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	
	virtual bool SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
		OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData) override;

	virtual bool GetStringAbnormalityTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) override;

private:
	void GetTimeString(float InTimeMin, float InTimeMax, OUT FString& OutStr);
};
