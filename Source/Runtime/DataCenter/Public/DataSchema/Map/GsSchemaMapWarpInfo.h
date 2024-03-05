/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsSchemaMapWarpInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapWarpInfo"))
struct DATACENTER_API FGsSchemaMapWarpInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="warpId"))
	int64 warpId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="shapeId"))
	FGsSchemaShapeDataRow shapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sourceSpot"))
	FGsSchemaMapWarpSpot sourceSpot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="destSpot"))
	FGsSchemaMapWarpSpot destSpot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="WarpType"))
	WarpType WarpType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ClientGameId"))
	int64 ClientGameId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
public:
	FGsSchemaMapWarpInfo();
	
protected:
	bool operator== (const FGsSchemaMapWarpInfo& __Other) const
	{
		return FGsSchemaMapWarpInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
