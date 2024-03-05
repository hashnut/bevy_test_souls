/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaArcasBless.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/XpBuff/BSGsArcasBless"))
struct DATACENTER_API FGsSchemaArcasBless
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	uint8 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minPoint"))
	int32 minPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxPoint"))
	int32 maxPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityId"))
	int32 passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isItemEffectArcasPointAutoUse"))
	bool isItemEffectArcasPointAutoUse;
public:
	FGsSchemaArcasBless();
	
protected:
	bool operator== (const FGsSchemaArcasBless& __Other) const
	{
		return FGsSchemaArcasBless::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
