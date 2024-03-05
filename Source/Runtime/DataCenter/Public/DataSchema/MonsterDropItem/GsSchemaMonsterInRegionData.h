/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterInRegionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterDropItem/BSGsMonsterInRegionData"))
struct DATACENTER_API FGsSchemaMonsterInRegionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpawnGroupId"))
	FGsSchemaSpawnDataRow SpawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MonsterCount"))
	int32 MonsterCount;
public:
	FGsSchemaMonsterInRegionData();
	
protected:
	bool operator== (const FGsSchemaMonsterInRegionData& __Other) const
	{
		return FGsSchemaMonsterInRegionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
