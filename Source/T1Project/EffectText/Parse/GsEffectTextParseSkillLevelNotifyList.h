// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifySet;
struct FGsSchemaEffectTextData;

/**
 * 노티 id 리스트
 * - int[n] : notifyId
 */
class T1PROJECT_API FGsEffectTextParseSkillLevelNotifyList final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillLevelNotifyList(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillLevelNotifyList() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_LEVEL_NOTIFY_LIST; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetData(const struct FGsSchemaSkillLevelInfo* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
