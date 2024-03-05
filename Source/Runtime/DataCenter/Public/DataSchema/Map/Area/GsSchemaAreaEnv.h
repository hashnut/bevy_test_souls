/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "DataSchema/Map/Area/GsSchemaWeatherSchedule.h"
#include "GsSchemaAreaEnv.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Area/BsGsAreaEnv"))
struct DATACENTER_API FGsSchemaAreaEnv
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weatherSchedule", Tooltip))
	FGsSchemaWeatherScheduleRow weatherSchedule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="audioPath", Tooltip))
	FGsSchemaSoundResDataRow audioPath;
	
public:
	FGsSchemaAreaEnv();
	
protected:
	bool operator== (const FGsSchemaAreaEnv& __Other) const
	{
		return FGsSchemaAreaEnv::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
