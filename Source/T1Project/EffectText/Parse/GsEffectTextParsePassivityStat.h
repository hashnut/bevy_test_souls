// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"
#include "EffectText/Parse/GsEffectTextParsePassivityBase.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaPassivitySet;
struct FGsSchemaPassivityEffectStatParam;

/**
 * FGsStatHelper를 이용해 스탯을 찍는다.
 * Condition 유무에 따라 저장되는 길이가 다름에 유의 (FGsEffectTextParsePassivityBase) 로직 참고
 * 
 * - 표기: {조건} 스탯 값
 * - 예: 조건 있을 때: HP 10% 이하일 때, 방어력 +10
 * - 예: 조건 없을 때: 회피율 +10%
 * 
 * - int[0] : Condition유무 (0: 없음, 1: 있음)
 * Condition이 있으면 n = 5, 없으면 n = 1
 * - int[n] : StatType
 * - int[n+1] : 값
 */
class T1PROJECT_API FGsEffectTextParsePassivityStat final : public FGsEffectTextParsePassivityBase
{
public:
	explicit FGsEffectTextParsePassivityStat(class FGsEffectTextParser* InParser)
		: FGsEffectTextParsePassivityBase(InParser)
	{
	}

	virtual ~FGsEffectTextParsePassivityStat() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::PASSIVITY_STAT; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	virtual bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData) override;

	virtual bool SetData(const FGsSchemaPassivitySet* InData, int32 InIndex, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

protected:
	void SetDataInter(const FGsSchemaPassivityEffectStatParam& InData, OUT FGsSchemaEffectTextData& OutData);
	void GetData(const FGsSchemaEffectTextData* InData, OUT TPair<StatType, int32>& OutData);
	bool ShowStatValue(StatType InType) const;
};
