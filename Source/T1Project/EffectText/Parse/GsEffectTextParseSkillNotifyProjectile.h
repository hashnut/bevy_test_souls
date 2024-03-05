// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifyElement;

/**
 * 
 * - 표기: 피해량의 n% HP로 흡수
 * - text[0] : effectText. 이 정보 있으면 다른 것 무시하고 무조건 이걸 찍음
 * //- float[0] : lifeTime
 * //- float[1] : tickInterval
 * - int[0] : SkillNotifySetId 
 * //- int[1] : actionInfoIndex (0부터 시작, 1부터는 GetStringInter 정보 안붙임)
 */
class T1PROJECT_API FGsEffectTextParseSkillNotifyProjectile final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillNotifyProjectile(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillNotifyProjectile() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_NOTIFY_PROJECTILE; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetSkillNotifyElementData(const FGsSchemaSkillNotifyElement* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

//private:
//	void GetStringInter(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
};
