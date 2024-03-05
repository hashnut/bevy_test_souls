/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcBalanceLevelStat.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcBalanceLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcBalanceLevel"))
struct DATACENTER_API FGsSchemaNpcBalanceLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statInfo"))
	TArray<FGsSchemaNpcBalanceLevelStat> statInfo;
public:
	FGsSchemaNpcBalanceLevel();
	
protected:
	bool operator== (const FGsSchemaNpcBalanceLevel& __Other) const
	{
		return FGsSchemaNpcBalanceLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
