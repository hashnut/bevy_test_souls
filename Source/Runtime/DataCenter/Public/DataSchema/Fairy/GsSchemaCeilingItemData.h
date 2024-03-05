/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCeilingItemData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsCeilingItemData"))
struct DATACENTER_API FGsSchemaCeilingItemData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ceilingItemId"))
	FGsSchemaItemCommonRow ceilingItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ceilingItemCount"))
	int32 ceilingItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="만분률", DisplayName="ceilingItemRate"))
	float ceilingItemRate;
public:
	FGsSchemaCeilingItemData();
	
protected:
	bool operator== (const FGsSchemaCeilingItemData& __Other) const
	{
		return FGsSchemaCeilingItemData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
