/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "GsSchemaEffectTextAbnormalityLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EffectText/BSGsEffectTextAbnormalityLevel"))
struct DATACENTER_API FGsSchemaEffectTextAbnormalityLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dataList"))
	TArray<FGsSchemaEffectTextData> dataList;
public:
	FGsSchemaEffectTextAbnormalityLevel();
	
protected:
	bool operator== (const FGsSchemaEffectTextAbnormalityLevel& __Other) const
	{
		return FGsSchemaEffectTextAbnormalityLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
