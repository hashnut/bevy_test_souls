/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "EffectText/EGsEffectTextParseType.h"
#include "GsSchemaEffectTextData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EffectText/BSGsEffectTextData"))
struct DATACENTER_API FGsSchemaEffectTextData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="parseType", Tooltip))
	EGsEffectTextParseType parseType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textParams", Tooltip))
	TArray<FText> textParams;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="intParams", Tooltip))
	TArray<int32> intParams;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="floatParams", Tooltip))
	TArray<float> floatParams;
	
public:
	FGsSchemaEffectTextData();
	
protected:
	bool operator== (const FGsSchemaEffectTextData& __Other) const
	{
		return FGsSchemaEffectTextData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
