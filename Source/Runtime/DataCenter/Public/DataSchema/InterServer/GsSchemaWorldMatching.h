/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/GsSchemaWorldMatchingData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMatching.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsWorldMatching"))
struct DATACENTER_API FGsSchemaWorldMatching
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="planetId"))
	int64 planetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldMatchingData"))
	TArray<FGsSchemaWorldMatchingData> worldMatchingData;
public:
	FGsSchemaWorldMatching();
	
protected:
	bool operator== (const FGsSchemaWorldMatching& __Other) const
	{
		return FGsSchemaWorldMatching::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
