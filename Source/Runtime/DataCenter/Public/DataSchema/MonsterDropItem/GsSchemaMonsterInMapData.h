/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/MonsterDropItem/GsSchemaMonsterInRegionData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterInMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterDropItem/BSGsMonsterInMapData"))
struct DATACENTER_API FGsSchemaMonsterInMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MapId"))
	int32 MapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MonsterInRegionDataList"))
	TArray<FGsSchemaMonsterInRegionData> MonsterInRegionDataList;
public:
	FGsSchemaMonsterInMapData();
	
protected:
	bool operator== (const FGsSchemaMonsterInMapData& __Other) const
	{
		return FGsSchemaMonsterInMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
