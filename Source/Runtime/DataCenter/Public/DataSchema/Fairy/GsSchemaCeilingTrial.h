/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCeilingTrial.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsCeilingTrial"))
struct DATACENTER_API FGsSchemaCeilingTrial
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="trialPhase"))
	int32 trialPhase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="needItemCount"))
	int32 needItemCount;
public:
	FGsSchemaCeilingTrial();
	
protected:
	bool operator== (const FGsSchemaCeilingTrial& __Other) const
	{
		return FGsSchemaCeilingTrial::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
