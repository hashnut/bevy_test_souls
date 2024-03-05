/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityCauseInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectInvokeAbnormalityParam.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityEffectInvokeAbnormalityParam"))
struct DATACENTER_API FGsSchemaPassivityEffectInvokeAbnormalityParam
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ratePct"))
	int32 ratePct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cause"))
	PassivityCauseType cause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeInfo"))
	FGsSchemaPassivityCauseInfo causeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="target"))
	PassivityInvokeTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="abnormalityId"))
	FGsSchemaAbnormalitySetRow abnormalityId;
public:
	FGsSchemaPassivityEffectInvokeAbnormalityParam();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInvokeAbnormalityParam& __Other) const
	{
		return FGsSchemaPassivityEffectInvokeAbnormalityParam::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
