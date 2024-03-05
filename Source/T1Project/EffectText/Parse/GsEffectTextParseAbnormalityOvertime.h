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
 * AbnormalityEffectType 중 OVERTIME 형태의 텍스트 기록
 * - int[0] : FGsSchemaAbnormalityIndexSet::AbnormalityEffectType
 * 
 * // 이하 살기일 때만 수집
 * - int[1] : FGsSchemaAbnormalityOvertime::targetType (SkillTargetType)
 * - int[2] : FGsSchemaAbnormalityOvertime::targetStat(StatType)
 * - int[3] : FGsSchemaAbnormalityOvertime::value 값
 * - int[4] : FGsSchemaAbnormalityOvertime::effectMethodType (AbnormalityEffectAddSubMethodType)
 * - int[5] : FGsSchemaAbnormalityOvertime::damageCategory (DamageStatType)
 *
 * - float[0] : durationMin
 * - float[1] : durationMax 
 * 
 * <OVERTIME_DAMAGE, OVERTIME_HOT>
 * - {abnormalityName} : {time}
 * - 예시: 출혈 2초
 * <OVERTIME_SENSITIVE>
 * - {abnormalityName} : {time}간 초당 {target} {type} {value}의 살기 획득
 * - 예시: 살기 2초간 초당 시전자 살기의 1의 살기 획득 
 * 
 *   - time: min~max초/min초
 *   - target: 출력 안함/대상/시전자
 *   - type: 출력 안함/최대 체력의/현재 체력의/공격력의/살기의
 *   - value: 값/만분율
 *   - targetStat: HP/MP
 *   - result: 피해/회복
 *   - limit: 출력 안함/최대 {0}의 피해
 */
class T1PROJECT_API FGsEffectTextParseAbnormalityOvertime final : public FGsEffectTextParseBase
{
public:

	enum EGsETAbOverTime : uint8
	{
		ET_EffectType = 0,

		// AbnormalityEffectType::OVERTIME_SENSITIVE일 때만 수집하는 정보
		ET_TargetType = 1,
		ET_StatType = 2,
		ET_StatValue = 3,
		ET_EffectMethod = 4,
		ET_DamageStat = 5,
	};

public:
	explicit FGsEffectTextParseAbnormalityOvertime(class FGsEffectTextParser* InParser)
		: FGsEffectTextParseBase(InParser)
	{
	}

	virtual ~FGsEffectTextParseAbnormalityOvertime() {}

public:
	virtual EGsEffectTextParseType GetParseType() const override { return EGsEffectTextParseType::ABNORMALITY_OVERTIME; }
	virtual bool IsValidData(const FGsSchemaEffectTextData* InData) const override;

	virtual bool GetString(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<FString>& OutData) override;
	virtual bool GetStringPair(const FGsSchemaEffectTextData* InData, int InLevel, int32 InOption, OUT TArray<TPair<FText, FText>>& OutData) override;
	
	virtual bool SetData(const FGsSchemaAbnormalitySet* InData, const FGsSchemaAbnormalityIndexSet* InIndexSet,
		OUT TArray<FGsSchemaEffectTextAbnormalityLevel>& OutData) override;

private:
	void GetStringInter(const FGsSchemaEffectTextData* InData, OUT FText& OutText);

	void GetTimeString(float InTimeMin, float InTimeMax, OUT FString& OutStr);
	void GetTargetText(const FGsSchemaEffectTextData* InData, OUT FText& OutText);
	void GetEffectResultText(const FGsSchemaEffectTextData* InData, OUT FText& OutText);
};
