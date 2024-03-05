/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRandomSpawnGroupInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsRandomSpawnGroupInfo"))
struct DATACENTER_API FGsSchemaRandomSpawnGroupInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="spawnGroupId"))
	int32 spawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomValue"))
	int32 randomValue;
public:
	FGsSchemaRandomSpawnGroupInfo();
	
protected:
	bool operator== (const FGsSchemaRandomSpawnGroupInfo& __Other) const
	{
		return FGsSchemaRandomSpawnGroupInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
