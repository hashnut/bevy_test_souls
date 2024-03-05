/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeProbabilityItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeProbabilityItem"))
struct DATACENTER_API FGsSchemaCostumeProbabilityItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="costumeId"))
	FGsSchemaCostumeCommonRow costumeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int64 probability;
public:
	FGsSchemaCostumeProbabilityItem();
	
protected:
	bool operator== (const FGsSchemaCostumeProbabilityItem& __Other) const
	{
		return FGsSchemaCostumeProbabilityItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
