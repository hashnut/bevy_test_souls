/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyData"))
struct DATACENTER_API FGsSchemaFairyData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyId", Tooltip))
	int32 fairyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="amount", Tooltip))
	int32 amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equip", Tooltip))
	bool equip;
public:
	FGsSchemaFairyData();
	
protected:
	bool operator== (const FGsSchemaFairyData& __Other) const
	{
		return FGsSchemaFairyData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
