/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardBox.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Reward/BSGsRewardBox"))
struct DATACENTER_API FGsSchemaRewardBox
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardBoxId"))
	int64 rewardBoxId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="백만분율", DisplayName="rate"))
	int32 rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="rewardBoxId와 같음 참고 및 편의용", DisplayName="rewardBoxDataRow"))
	FGsSchemaRewardBoxDataRow rewardBoxDataRow;
public:
	FGsSchemaRewardBox();
	
protected:
	bool operator== (const FGsSchemaRewardBox& __Other) const
	{
		return FGsSchemaRewardBox::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
