/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeProbability.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeSummonItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeSummonItem"))
struct DATACENTER_API FGsSchemaCostumeSummonItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="probabilityId"))
	FGsSchemaCostumeProbabilityRow probabilityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int64 probability;
public:
	FGsSchemaCostumeSummonItem();
	
protected:
	bool operator== (const FGsSchemaCostumeSummonItem& __Other) const
	{
		return FGsSchemaCostumeSummonItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
