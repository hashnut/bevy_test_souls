/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCultureTimeText.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Time/BSGsCultureTimeText"))
struct DATACENTER_API FGsSchemaCultureTimeText
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="culture"))
	FString culture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="text"))
	FText text;
public:
	FGsSchemaCultureTimeText();
	
protected:
	bool operator== (const FGsSchemaCultureTimeText& __Other) const
	{
		return FGsSchemaCultureTimeText::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
