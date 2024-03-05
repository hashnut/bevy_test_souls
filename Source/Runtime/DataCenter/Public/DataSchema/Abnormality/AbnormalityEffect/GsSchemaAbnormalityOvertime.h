/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityOvertime.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityEffect/BSGsAbnormalityOvertime"))
struct DATACENTER_API FGsSchemaAbnormalityOvertime
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetType"))
	SkillTargetType targetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetStat"))
	FGsStatTypeNameSelector targetStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectMethodType"))
	AbnormalityEffectAddSubMethodType effectMethodType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="value"))
	int32 value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="damageCategory"))
	DamageStatType damageCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxTickDamage"))
	int32 maxTickDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxTickDamagePC"))
	int32 maxTickDamagePC;
public:
	FGsSchemaAbnormalityOvertime();
	
protected:
	bool operator== (const FGsSchemaAbnormalityOvertime& __Other) const
	{
		return FGsSchemaAbnormalityOvertime::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
