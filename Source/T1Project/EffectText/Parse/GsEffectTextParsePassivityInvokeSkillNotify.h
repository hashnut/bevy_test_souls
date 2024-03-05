// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/Parse/GsEffectTextParsePassivityBase.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaPassivitySet;

/**
 * SkillNotify를 발동하는 패시비티
* Condition 유무에 따라 저장되는 길이가 다름에 유의. FGsEffectTextParsePassivityBase 공통 로직
 * - 표기: {SkillNotify규칙}
 * 
 * - int: FGsEffectTextParsePassivityBase의 주석 참고
 * - float[0] : FGsSchemaPassivityEffectInvokeSkillNotifyParam::cooldownTime
 */
class T1PROJECT_API FGsEffectTextParsePassivityInvokeSkillNotify final : public FGsEffectTextParsePassivityBase
{
public:
	explicit FGsEffectTextParsePassivityInvokeSkillNotify(class FGsEffectTextParser* InParser)
		: FGsEffectTextParsePassivityBase(InParser)
	{
	}

	virtual ~FGsEffectTextParsePassivityInvokeSkillNotify() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::PASSIVITY_INVOKE_SKILL_NOTIFY; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	virtual bool GetStringCollection(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT FGsEffectTextCollection& OutData) override;

	virtual void GetStringPassivityCauseAndRate(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) override;
	virtual void GetStringPassivityTarget(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) override;
	virtual void GetStringPassivityCoolTime(const FGsSchemaEffectTextData* InData, int32 InOption, OUT FString& OutData) override;

public:
	virtual bool SetData(const FGsSchemaPassivitySet* InData, int32 InIndex, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
