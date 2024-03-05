/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCraftSupplyInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SupplyInfo/BSGsCraftSupplyInfo"))
struct DATACENTER_API FGsSchemaCraftSupplyInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="craftIds"))
	TSet<int32> craftIds;
public:
	FGsSchemaCraftSupplyInfo();
	
protected:
	bool operator== (const FGsSchemaCraftSupplyInfo& __Other) const
	{
		return FGsSchemaCraftSupplyInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
