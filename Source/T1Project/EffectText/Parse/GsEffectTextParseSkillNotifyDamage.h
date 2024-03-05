// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifySet;
struct FGsSchemaSkillNotifyElement;

/**
 * 노티파이의 데미지 정보. 패시비티의 InvokeSkillNotify에서만 쓰고, 스킬은 FGsEffectTextParseSkillLevelDamage 사용
 * - FGsEffectTextFuncSkillDamage의 주석 참고 
 */
class T1PROJECT_API FGsEffectTextParseSkillNotifyDamage final : public FGsEffectTextParseBase
{
public:
	explicit FGsEffectTextParseSkillNotifyDamage(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillNotifyDamage() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_NOTIFY_DAMAGE; }	
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;
	
	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	// 주의: 노티파이 하나에 대한 정보임
	virtual bool SetData(const FGsSchemaSkillNotifySet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;
};
