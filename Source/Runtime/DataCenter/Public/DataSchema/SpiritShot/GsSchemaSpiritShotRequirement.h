/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotRequirement.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotRequirement"))
struct DATACENTER_API FGsSchemaSpiritShotRequirement
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemIds"))
	TArray<int32> itemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="amount"))
	int32 amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="modifyItemAmount"))
	int32 modifyItemAmount;
public:
	FGsSchemaSpiritShotRequirement();
	
protected:
	bool operator== (const FGsSchemaSpiritShotRequirement& __Other) const
	{
		return FGsSchemaSpiritShotRequirement::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
