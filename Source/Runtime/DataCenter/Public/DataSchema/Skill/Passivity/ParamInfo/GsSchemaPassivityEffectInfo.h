/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectConditionInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityEffectInfo"))
struct DATACENTER_API FGsSchemaPassivityEffectInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionInfo"))
	FGsSchemaPassivityEffectConditionInfo conditionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="executeConditionId"))
	int32 executeConditionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
public:
	FGsSchemaPassivityEffectInfo();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInfo& __Other) const
	{
		return FGsSchemaPassivityEffectInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
