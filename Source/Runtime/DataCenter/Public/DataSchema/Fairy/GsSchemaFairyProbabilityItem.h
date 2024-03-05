/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyProbabilityItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyProbabilityItem"))
struct DATACENTER_API FGsSchemaFairyProbabilityItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fairyId"))
	FGsSchemaFairyCommonRow fairyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int64 probability;
public:
	FGsSchemaFairyProbabilityItem();
	
protected:
	bool operator== (const FGsSchemaFairyProbabilityItem& __Other) const
	{
		return FGsSchemaFairyProbabilityItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
