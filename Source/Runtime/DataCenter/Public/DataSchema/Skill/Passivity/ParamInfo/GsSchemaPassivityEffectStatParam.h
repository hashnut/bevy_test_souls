/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaPassivityEffectStatParam.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityEffectStatParam"))
struct DATACENTER_API FGsSchemaPassivityEffectStatParam
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	FGsStatTypeNameSelector type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value"))
	int32 value;
public:
	FGsSchemaPassivityEffectStatParam();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectStatParam& __Other) const
	{
		return FGsSchemaPassivityEffectStatParam::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
