/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityStat.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityEffect/BSGsAbnormalityStat"))
struct DATACENTER_API FGsSchemaAbnormalityStat
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="methodType"))
	AbnormalityEffectStatMethodType methodType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="proportionTargetType"))
	SkillTargetType proportionTargetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="baseStat"))
	FGsStatTypeNameSelector baseStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionValue"))
	int32 conditionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="affectedStat"))
	FGsStatTypeNameSelector affectedStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="affectedValue"))
	int32 affectedValue;
public:
	FGsSchemaAbnormalityStat();
	
protected:
	bool operator== (const FGsSchemaAbnormalityStat& __Other) const
	{
		return FGsSchemaAbnormalityStat::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
