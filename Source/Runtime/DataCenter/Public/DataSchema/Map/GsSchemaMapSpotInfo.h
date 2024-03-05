/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "GsSchemaMapSpotInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapSpotInfo"))
struct DATACENTER_API FGsSchemaMapSpotInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaSpotDataRow mapId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spotIndex", Tooltip))
	int64 spotIndex;
	
public:
	FGsSchemaMapSpotInfo();
	
protected:
	bool operator== (const FGsSchemaMapSpotInfo& __Other) const
	{
		return FGsSchemaMapSpotInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
