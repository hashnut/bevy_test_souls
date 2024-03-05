/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Common/GsSchemaMinMaxInteger.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterSpawnRate.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterSpawnRateGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Wave/BSGsMonsterSpawnRateGroup"))
struct DATACENTER_API FGsSchemaMonsterSpawnRateGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcSpawnList"))
	TArray<FGsSchemaMonsterSpawnRate> npcSpawnList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnCount"))
	FGsSchemaMinMaxInteger spawnCount;
public:
	FGsSchemaMonsterSpawnRateGroup();
	
protected:
	bool operator== (const FGsSchemaMonsterSpawnRateGroup& __Other) const
	{
		return FGsSchemaMonsterSpawnRateGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
