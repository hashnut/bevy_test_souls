// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaPassivitySet;

/**
 * 이름만 찍는다
 * - text[0] : 이름
 *
 * - 예: 워프불가
 */
class T1PROJECT_API FGsEffectTextParsePassivityDefault final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParsePassivityDefault(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParsePassivityDefault() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::PASSIVITY_DEFAULT; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetData(const FGsSchemaPassivitySet* InData, int32 InIndex, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
