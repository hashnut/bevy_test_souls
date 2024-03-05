/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropProbGroupInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Drop/BSGsDropProbGroupInfo"))
struct DATACENTER_API FGsSchemaDropProbGroupInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryMain"))
	ItemCategoryMain categoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categorySub"))
	ItemCategorySub categorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="prob"))
	int32 prob;
public:
	FGsSchemaDropProbGroupInfo();
	
protected:
	bool operator== (const FGsSchemaDropProbGroupInfo& __Other) const
	{
		return FGsSchemaDropProbGroupInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
