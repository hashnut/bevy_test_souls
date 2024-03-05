/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionItemSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionItemSet"))
struct DATACENTER_API FGsSchemaOptionItemSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionSelectType"))
	EGsOptionSelectType OptionSelectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionItemType"))
	EGsOptionItem OptionItemType;
public:
	FGsSchemaOptionItemSet();
	
protected:
	bool operator== (const FGsSchemaOptionItemSet& __Other) const
	{
		return FGsSchemaOptionItemSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
