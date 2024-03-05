/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMatchingData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsWorldMatchingData"))
struct DATACENTER_API FGsSchemaWorldMatchingData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldId"))
	FGsSchemaWorldDataRow worldId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="targetWorldId"))
	FGsSchemaWorldDataRow targetWorldId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bossSpawnWeek"))
	TArray<int32> bossSpawnWeek;
public:
	FGsSchemaWorldMatchingData();
	
protected:
	bool operator== (const FGsSchemaWorldMatchingData& __Other) const
	{
		return FGsSchemaWorldMatchingData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
