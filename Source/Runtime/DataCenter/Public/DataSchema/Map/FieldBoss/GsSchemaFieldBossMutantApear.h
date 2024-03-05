/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFieldBossMutantApear.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/FieldBoss/BSGsFieldBossMutantApear"))
struct DATACENTER_API FGsSchemaFieldBossMutantApear
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int32 probability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fieldbossDataId"))
	int32 fieldbossDataId;
public:
	FGsSchemaFieldBossMutantApear();
	
protected:
	bool operator== (const FGsSchemaFieldBossMutantApear& __Other) const
	{
		return FGsSchemaFieldBossMutantApear::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
