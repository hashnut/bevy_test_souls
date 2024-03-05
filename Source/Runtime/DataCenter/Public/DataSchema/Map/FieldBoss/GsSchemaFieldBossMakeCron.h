/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFieldBossMakeCron.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/FieldBoss/BSGsFieldBossMakeCron"))
struct DATACENTER_API FGsSchemaFieldBossMakeCron
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="start"))
	FString start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationMinutes"))
	int32 durationMinutes;
public:
	FGsSchemaFieldBossMakeCron();
	
protected:
	bool operator== (const FGsSchemaFieldBossMakeCron& __Other) const
	{
		return FGsSchemaFieldBossMakeCron::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
