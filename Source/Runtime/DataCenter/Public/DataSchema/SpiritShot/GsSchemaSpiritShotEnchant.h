/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotEnchant.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotEnchant"))
struct DATACENTER_API FGsSchemaSpiritShotEnchant
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultSuccessRate"))
	int32 defaultSuccessRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxSuccessRate"))
	int32 maxSuccessRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelFallRate"))
	int32 levelFallRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rateModifyValue"))
	int32 rateModifyValue;
public:
	FGsSchemaSpiritShotEnchant();
	
protected:
	bool operator== (const FGsSchemaSpiritShotEnchant& __Other) const
	{
		return FGsSchemaSpiritShotEnchant::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
