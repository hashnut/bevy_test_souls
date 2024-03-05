// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/Parse/GsEffectTextParsePassivityBase.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaPassivitySet;

/**
 * 특정 스킬의 레벨을 올리는 타입
* Condition 유무에 따라 저장되는 길이가 다름에 유의. FGsEffectTextParsePassivityBase 공통 로직
 * - 표기: {WeaponType} {SkillName} Lv.+{Value}
 * 
 * Condition이 있으면 n = 5, 없으면 n = 1
* - int[n] : userSkillSetId
* - int[n+1] : value
 */
class T1PROJECT_API FGsEffectTextParsePassivitySkillUp final : public FGsEffectTextParsePassivityBase
{
public:
	explicit FGsEffectTextParsePassivitySkillUp(class FGsEffectTextParser* InParser)
		: FGsEffectTextParsePassivityBase(InParser)
	{
	}

	virtual ~FGsEffectTextParsePassivitySkillUp() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::PASSIVITY_SKILL_UP; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetData(const FGsSchemaPassivitySet* InData, int32 InIndex, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

private:
	bool GetStringInter(const FGsSchemaEffectTextData* InData, OUT FString& OutData);
};
