/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMinMaxInteger.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Common/BSGsMinMaxInteger"))
struct DATACENTER_API FGsSchemaMinMaxInteger
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Min"))
	int32 Min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Max"))
	int32 Max;
public:
	FGsSchemaMinMaxInteger();
	
protected:
	bool operator== (const FGsSchemaMinMaxInteger& __Other) const
	{
		return FGsSchemaMinMaxInteger::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
