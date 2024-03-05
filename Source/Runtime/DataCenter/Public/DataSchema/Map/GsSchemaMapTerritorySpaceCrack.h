/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapTerritorySpaceCrack.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapTerritorySpaceCrack"))
struct DATACENTER_API FGsSchemaMapTerritorySpaceCrack
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="territoryId"))
	int64 territoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fenceId"))
	int64 fenceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="center"))
	FVector center;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="vertexList"))
	TArray<FVector> vertexList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="degree"))
	float degree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="radius"))
	float radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="envEvent"))
	EGsEnvEvent envEvent;
public:
	FGsSchemaMapTerritorySpaceCrack();
	
protected:
	bool operator== (const FGsSchemaMapTerritorySpaceCrack& __Other) const
	{
		return FGsSchemaMapTerritorySpaceCrack::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
