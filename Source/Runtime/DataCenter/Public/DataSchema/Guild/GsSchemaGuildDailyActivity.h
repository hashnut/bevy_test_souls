/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildDailyActivity.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildDailyActivity"))
struct DATACENTER_API FGsSchemaGuildDailyActivity
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	GuildDailyActivityType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획참고용", DisplayName="note"))
	FString note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 재화 종류", DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 재화 수량", DisplayName="currencyAmount"))
	int32 currencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="활동 시 받는 기여도", DisplayName="rewardContribution"))
	int32 rewardContribution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="활동시 받는 보상", DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
public:
	FGsSchemaGuildDailyActivity();
	
protected:
	bool operator== (const FGsSchemaGuildDailyActivity& __Other) const
	{
		return FGsSchemaGuildDailyActivity::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
