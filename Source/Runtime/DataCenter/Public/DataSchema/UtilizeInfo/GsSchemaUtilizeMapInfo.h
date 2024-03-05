/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUtilizeMapInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UtilizeInfo/BSGsUtilizeMapInfo"))
struct DATACENTER_API FGsSchemaUtilizeMapInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
public:
	FGsSchemaUtilizeMapInfo();
	
protected:
	bool operator== (const FGsSchemaUtilizeMapInfo& __Other) const
	{
		return FGsSchemaUtilizeMapInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
