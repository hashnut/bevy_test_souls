/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotEffect"))
struct DATACENTER_API FGsSchemaSpiritShotEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectPath"))
	FSoftObjectPath effectPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectScale"))
	int32 effectScale;
public:
	FGsSchemaSpiritShotEffect();
	
protected:
	bool operator== (const FGsSchemaSpiritShotEffect& __Other) const
	{
		return FGsSchemaSpiritShotEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
