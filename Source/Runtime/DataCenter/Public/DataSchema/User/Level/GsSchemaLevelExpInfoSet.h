/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLevelExpInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGSLevelExpInfoSet"))
struct DATACENTER_API FGsSchemaLevelExpInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Level", Tooltip))
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="TotalExp", Tooltip))
	int64 TotalExp;
public:
	FGsSchemaLevelExpInfoSet();
	
protected:
	bool operator== (const FGsSchemaLevelExpInfoSet& __Other) const
	{
		return FGsSchemaLevelExpInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
