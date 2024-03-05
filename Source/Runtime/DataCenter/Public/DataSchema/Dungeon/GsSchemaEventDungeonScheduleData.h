/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventDungeonScheduleData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Dungeon/BSGsEventDungeonScheduleData"))
struct DATACENTER_API FGsSchemaEventDungeonScheduleData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openCycling"))
	FString openCycling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playTimeMinute"))
	int32 playTimeMinute;
public:
	FGsSchemaEventDungeonScheduleData();
	
protected:
	bool operator== (const FGsSchemaEventDungeonScheduleData& __Other) const
	{
		return FGsSchemaEventDungeonScheduleData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
