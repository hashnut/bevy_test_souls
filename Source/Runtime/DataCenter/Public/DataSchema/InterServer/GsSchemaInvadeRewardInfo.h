/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInvadeRewardInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInvadeRewardInfo"))
struct DATACENTER_API FGsSchemaInvadeRewardInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hpConditionRate"))
	int32 hpConditionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardIdList"))
	TArray<FGsSchemaRewardDataRow> rewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionMin"))
	int32 contributionMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropObjectRadius"))
	float dropObjectRadius;
public:
	FGsSchemaInvadeRewardInfo();
	
protected:
	bool operator== (const FGsSchemaInvadeRewardInfo& __Other) const
	{
		return FGsSchemaInvadeRewardInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
