/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectConditionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityEffectConditionInfo"))
struct DATACENTER_API FGsSchemaPassivityEffectConditionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	PassivityConditionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="method"))
	PassivityConditionMethod method;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param1"))
	int32 param1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param2"))
	int32 param2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param3"))
	AbnormalityEffectType param3;
public:
	FGsSchemaPassivityEffectConditionInfo();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectConditionInfo& __Other) const
	{
		return FGsSchemaPassivityEffectConditionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
