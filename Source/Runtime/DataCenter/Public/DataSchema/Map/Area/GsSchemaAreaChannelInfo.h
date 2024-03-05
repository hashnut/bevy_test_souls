/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAreaChannelInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Area/BSGsAreaChannelInfo"))
struct DATACENTER_API FGsSchemaAreaChannelInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="initCount", Tooltip))
	int32 initCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxCount", Tooltip))
	int32 maxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxUserCount", Tooltip))
	int32 maxUserCount;
	
public:
	FGsSchemaAreaChannelInfo();
	
protected:
	bool operator== (const FGsSchemaAreaChannelInfo& __Other) const
	{
		return FGsSchemaAreaChannelInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
