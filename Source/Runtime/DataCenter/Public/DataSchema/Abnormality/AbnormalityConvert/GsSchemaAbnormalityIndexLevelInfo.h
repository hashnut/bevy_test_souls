/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityIndexLevelInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityConvert/BSGsAbnormalityIndexLevelInfo"))
struct DATACENTER_API FGsSchemaAbnormalityIndexLevelInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectRow"))
	FGsSchemaRowPoly effectRow;
public:
	FGsSchemaAbnormalityIndexLevelInfo();
	
protected:
	bool operator== (const FGsSchemaAbnormalityIndexLevelInfo& __Other) const
	{
		return FGsSchemaAbnormalityIndexLevelInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
