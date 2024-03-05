/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaArcasConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/XpBuff/BSGsArcasConfig"))
struct DATACENTER_API FGsSchemaArcasConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxArcasPoint"))
	int32 maxArcasPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pointCoefficientRate"))
	float pointCoefficientRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconPath"))
	TArray<FSoftObjectPath> iconPath;
public:
	FGsSchemaArcasConfig();
	
protected:
	bool operator== (const FGsSchemaArcasConfig& __Other) const
	{
		return FGsSchemaArcasConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
