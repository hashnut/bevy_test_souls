/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpotInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spot/BSGsSpotInfo"))
struct DATACENTER_API FGsSchemaSpotInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pos"))
	FVector pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dir"))
	int32 dir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="radius"))
	float radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isInTown"))
	bool isInTown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomDir"))
	bool randomDir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isExceptRandomTeleport"))
	bool isExceptRandomTeleport;
public:
	FGsSchemaSpotInfo();
	
protected:
	bool operator== (const FGsSchemaSpotInfo& __Other) const
	{
		return FGsSchemaSpotInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
