/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSendingOption.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/Condition/BSGsSendingOption"))
struct DATACENTER_API FGsSchemaSendingOption
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isEnsured"))
	bool isEnsured;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isImmediate"))
	bool isImmediate;
public:
	FGsSchemaSendingOption();
	
protected:
	bool operator== (const FGsSchemaSendingOption& __Other) const
	{
		return FGsSchemaSendingOption::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
