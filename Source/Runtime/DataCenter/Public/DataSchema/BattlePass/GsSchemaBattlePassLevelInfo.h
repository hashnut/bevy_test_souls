/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassLevelInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGsBattlePassLevelInfo"))
struct DATACENTER_API FGsSchemaBattlePassLevelInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exp"))
	int64 exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseCurrencyType"))
	CurrencyType levelPurchaseCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseCurrency"))
	int64 levelPurchaseCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="levelPurchaseItemId"))
	FGsSchemaItemCommonRow levelPurchaseItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseItemCount"))
	int32 levelPurchaseItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="levelBasicReward"))
	FGsSchemaRewardDataRow levelBasicReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="levelAdditionalReward"))
	FGsSchemaRewardDataRow levelAdditionalReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="0 이면 이전에 노출요구를 따름: 레벨 입력시는 그 레벨을 노출", DisplayName="displayLevelFlag"))
	bool displayLevelFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="icon 만 입력하면 됩니다", DisplayName="icon"))
	FSoftObjectPath icon;
public:
	FGsSchemaBattlePassLevelInfo();
	
protected:
	bool operator== (const FGsSchemaBattlePassLevelInfo& __Other) const
	{
		return FGsSchemaBattlePassLevelInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
