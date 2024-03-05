/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEnchantLevelList.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsEnchantLevelList"))
struct DATACENTER_API FGsSchemaEnchantLevelList
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantLevel"))
	int32 enchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="decomposeRewardId"))
	int32 decomposeRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantFailRewardId"))
	int32 enchantFailRewardId;
public:
	FGsSchemaEnchantLevelList();
	
protected:
	bool operator== (const FGsSchemaEnchantLevelList& __Other) const
	{
		return FGsSchemaEnchantLevelList::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
