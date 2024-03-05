/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTerritoryZoneShape.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsTerritoryZoneShape"))
struct DATACENTER_API FGsSchemaTerritoryZoneShape
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Center", Tooltip))
	FVector Center;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Radius", Tooltip))
	int32 Radius;
	
public:
	FGsSchemaTerritoryZoneShape();
	
protected:
	bool operator== (const FGsSchemaTerritoryZoneShape& __Other) const
	{
		return FGsSchemaTerritoryZoneShape::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
