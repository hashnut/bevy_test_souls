/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "GsSchemaMapWarpSpot.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapWarpSpot"))
struct DATACENTER_API FGsSchemaMapWarpSpot
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaSpotDataRow MapId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpotIndex", Tooltip))
	int64 SpotIndex;
	
public:
	FGsSchemaMapWarpSpot();
	
protected:
	bool operator== (const FGsSchemaMapWarpSpot& __Other) const
	{
		return FGsSchemaMapWarpSpot::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
