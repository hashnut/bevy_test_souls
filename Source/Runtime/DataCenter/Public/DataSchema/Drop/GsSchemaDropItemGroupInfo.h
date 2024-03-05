/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropItemGroupInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Drop/BSGsDropItemGroupInfo"))
struct DATACENTER_API FGsSchemaDropItemGroupInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	FGsSchemaItemCommonRow itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCountMin"))
	int32 itemCountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCountMax"))
	int32 itemCountMax;
public:
	FGsSchemaDropItemGroupInfo();
	
protected:
	bool operator== (const FGsSchemaDropItemGroupInfo& __Other) const
	{
		return FGsSchemaDropItemGroupInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
