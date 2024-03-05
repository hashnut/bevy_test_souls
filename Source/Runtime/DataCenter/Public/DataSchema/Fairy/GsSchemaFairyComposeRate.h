/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaCeilingItemData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyComposeRate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyComposeRate"))
struct DATACENTER_API FGsSchemaFairyComposeRate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="materialCount"))
	int32 materialCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="upgradeRate"))
	float upgradeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ceilingItemDataList"))
	TArray<FGsSchemaCeilingItemData> ceilingItemDataList;
public:
	FGsSchemaFairyComposeRate();
	
protected:
	bool operator== (const FGsSchemaFairyComposeRate& __Other) const
	{
		return FGsSchemaFairyComposeRate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
