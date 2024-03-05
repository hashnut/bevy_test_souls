/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMClassChangeEquipCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMClassChange/BSGsBMClassChangeEquipCategory"))
struct DATACENTER_API FGsSchemaBMClassChangeEquipCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="category"))
	ItemCategorySub category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxCount"))
	int32 maxCount;
public:
	FGsSchemaBMClassChangeEquipCategory();
	
protected:
	bool operator== (const FGsSchemaBMClassChangeEquipCategory& __Other) const
	{
		return FGsSchemaBMClassChangeEquipCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
