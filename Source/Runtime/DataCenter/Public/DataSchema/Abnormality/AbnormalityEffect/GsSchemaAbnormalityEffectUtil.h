/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityEffectUtil.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityEffect/BSGsAbnormalityEffectUtil"))
struct DATACENTER_API FGsSchemaAbnormalityEffectUtil
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="SkillNoityId"))
	FGsSchemaSkillNotifySetRow SkillNoityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowedClasses="AnimMontage", DisplayName="minHp"))
	int32 minHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="damageRate"))
	int32 damageRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="abnormalityId"))
	FGsSchemaAbnormalitySetRow abnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="반격 발동 조건 설정(AbnormalityEffectBuffCountHitType)", DisplayName="hitType"))
	AbnormalityEffectBuffCountHitType hitType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 반격 효과 발동 제한 설정(AbnormalityEffectBuffCountSkillGrade)", DisplayName="skillGrade"))
	AbnormalityEffectBuffCountSkillGrade skillGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="반격 효과 발동 확률 (만분율)", DisplayName="rate"))
	int32 rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="반격 효과 발동 내부 쿨타임", DisplayName="cooldown"))
	float cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="특정 대미지 제공 시 NotifyId 입력, 입력하지 않는 경우 받은 피해를 대미지로 제공", DisplayName="damage"))
	int32 damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최종 피격 대미지 감소율 (만분율)", DisplayName="damage_reduction_rate"))
	int32 damage_reduction_rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\'흡혈 비율\', 만 분률, 0이면 사용하지 않음, 데미지에 곱하여 시전자 HP를 증가. (기본 값: 0)", DisplayName="hpAbsorptionRate"))
	int32 hpAbsorptionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\'흡혈 비율 HP 회복 제한\', \'흡혈 비율\'에 의해 증가되는 최대 HP를 제한. (기본 값: 0)", DisplayName="hpLimitAbsorptionRate"))
	int32 hpLimitAbsorptionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\'흡혈\', 해당 수치만큼 시전자의 HP를 증가. (기본 값: 0)", DisplayName="hpAbsorption"))
	int32 hpAbsorption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="StatType::HP_BONUS_HEAL_RATE 의 영향을 받는지 여부. (기본 값: false)", DisplayName="hpAbsorptionAffectedByBonusRate"))
	bool hpAbsorptionAffectedByBonusRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공유 대미지 비율(만분율)", DisplayName="damageShareRate"))
	int32 damageShareRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공유 대미지 적용률(만분율)", DisplayName="damageShareApplicationRate"))
	int32 damageShareApplicationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공유 대미지 제한율(만분율)", DisplayName="damageShareLimitHpRate"))
	int32 damageShareLimitHpRate;
public:
	FGsSchemaAbnormalityEffectUtil();
	
protected:
	bool operator== (const FGsSchemaAbnormalityEffectUtil& __Other) const
	{
		return FGsSchemaAbnormalityEffectUtil::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
