/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCodexCategoryBalanceLevelData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Codex/SchemaOnly/BSGsCodexCategoryBalanceLevelData"))
struct DATACENTER_API FGsSchemaCodexCategoryBalanceLevelData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapGroupLevelUpCount"))
	int32 mapGroupLevelUpCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityId"))
	FGsSchemaPassivitySetRow passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isGlobalPassivity"))
	bool isGlobalPassivity;
public:
	FGsSchemaCodexCategoryBalanceLevelData();
	
protected:
	bool operator== (const FGsSchemaCodexCategoryBalanceLevelData& __Other) const
	{
		return FGsSchemaCodexCategoryBalanceLevelData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
