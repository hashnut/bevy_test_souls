/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemMagicalForgeData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/MagicForge/BSGsItemMagicalForgeData"))
struct DATACENTER_API FGsSchemaItemMagicalForgeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityId"))
	FGsSchemaPassivitySetRow passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costGoldAmount"))
	int64 costGoldAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int32 probability;
public:
	FGsSchemaItemMagicalForgeData();
	
protected:
	bool operator== (const FGsSchemaItemMagicalForgeData& __Other) const
	{
		return FGsSchemaItemMagicalForgeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
