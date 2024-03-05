/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/User/Level/GsSchemaLevelStatInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLevelStat.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGsLevelStat"))
struct DATACENTER_API FGsSchemaLevelStat
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Level"))
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="StatPoint"))
	int32 StatPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="StatInfo"))
	TArray<FGsSchemaLevelStatInfo> StatInfo;
public:
	FGsSchemaLevelStat();
	
protected:
	bool operator== (const FGsSchemaLevelStat& __Other) const
	{
		return FGsSchemaLevelStat::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
