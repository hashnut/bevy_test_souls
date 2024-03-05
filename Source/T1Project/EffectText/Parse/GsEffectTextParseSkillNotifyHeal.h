// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "EffectText/GsEffectTextParseBase.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"

struct FGsSchemaEffectTextData;
struct FGsSchemaSkillNotifySet;

/**
 * 스킬 노티파이 SkillNotifyType::VARY_HP_MP 일 때 정보
 * - 표기: 타입 값 회복/감소
 * - 예시: HP 100 회복
 * - int[0] : FGsSchemaSkillNotifyHeal::healStatType (DamageStatType)
 * - int[1] : fixedValue
 * - int[2] : ratioValue
 * - int[3] : FGsSchemaSkillNotifyHeal::healRatioValueType (HealRatioValueType)
 * - int[4] : FGsSchemaSkillNotifyHeal::target (SkillTargetType)
 * 
 * <HealRatioValueType::NONE일 경우>
 * - {statType} {value} {effect}
 * <HealRatioValueType::NONE이 아닐 경우(비울표기)>
 * - (fixedValue가 0이 아닌 경우) 위의 값도 같이 출력 
 * - {target} {healRatioValueType} {statType} {value} {effect}
 * 
 */
class T1PROJECT_API FGsEffectTextParseSkillNotifyHeal final : public FGsEffectTextParseBase
{
public:
	enum EGsETSkillHealParam : uint8
	{
		ET_DamageStatType = 0,
		ET_FixedValue = 1,
		ET_RatioValue = 2,
		ET_RatioValueType = 3,
		ET_TargetType = 4,

		ET_MaxParam,
	};

public:
	explicit FGsEffectTextParseSkillNotifyHeal(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}
	virtual ~FGsEffectTextParseSkillNotifyHeal() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::SKILL_NOTIFY_HEAL; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;

	virtual bool SetData(const FGsSchemaSkillNotifySet* InData, OUT TArray<FGsSchemaEffectTextData>& OutData) override;

protected:
	//void AddStringValue(DamageStatType InType, int32 InValue, OUT FString& OutString);

	void GetStringInter(const FGsSchemaEffectTextData* InData, OUT FString& OutString);
	void GetStatValueString(DamageStatType InType, int32 InValue, bool bIsRatio, OUT FString& OutString);
};

// 기획서에 따른 내용정리가 필요하다.
