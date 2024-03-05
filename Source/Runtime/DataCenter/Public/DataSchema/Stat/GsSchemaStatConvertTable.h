/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/Data/GsItemStatData.h"
#include "GsSchemaStatConvertTable.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsStatConvertTable"))
struct DATACENTER_API FGsSchemaStatConvertTable
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="baseValue", Tooltip))
	int32 baseValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statList", Tooltip))
	TArray<FGsItemStatData> statList;
	
public:
	FGsSchemaStatConvertTable();
	
protected:
	bool operator== (const FGsSchemaStatConvertTable& __Other) const
	{
		return FGsSchemaStatConvertTable::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
