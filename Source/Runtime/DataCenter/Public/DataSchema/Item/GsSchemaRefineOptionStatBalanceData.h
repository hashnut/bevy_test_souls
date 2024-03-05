/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRefineOptionStatBalanceData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsRefineOptionStatBalanceData"))
struct DATACENTER_API FGsSchemaRefineOptionStatBalanceData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValueMin", Tooltip))
	int32 statValueMin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValueMax", Tooltip))
	int32 statValueMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability", Tooltip))
	int32 probability;
	
public:
	FGsSchemaRefineOptionStatBalanceData();
	
protected:
	bool operator== (const FGsSchemaRefineOptionStatBalanceData& __Other) const
	{
		return FGsSchemaRefineOptionStatBalanceData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
