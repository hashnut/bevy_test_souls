/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassLevelBonusInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGSBattlePassLevelBonusInfo"))
struct DATACENTER_API FGsSchemaBattlePassLevelBonusInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="레벨 마지막에 보너스가 있는가 여부 : 없을 수도 있으므로", DisplayName="isPayBonus"))
	bool isPayBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="지급 카운트당 필요 초과 경험치", DisplayName="exceedPerExp"))
	int64 exceedPerExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="0 이면 max 없이 계속 지급", DisplayName="maxExp"))
	int64 maxExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bonusReward"))
	FGsSchemaRewardDataRow bonusReward;
public:
	FGsSchemaBattlePassLevelBonusInfo();
	
protected:
	bool operator== (const FGsSchemaBattlePassLevelBonusInfo& __Other) const
	{
		return FGsSchemaBattlePassLevelBonusInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
