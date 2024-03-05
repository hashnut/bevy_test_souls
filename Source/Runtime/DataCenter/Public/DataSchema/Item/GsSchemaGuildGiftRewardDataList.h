/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaGuildGiftRewardDataList.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsGuildGiftRewardDataList"))
struct DATACENTER_API FGsSchemaGuildGiftRewardDataList
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardNo"))
	int32 rewardNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="giveTo"))
	GuildGiftGiveType giveTo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemAmount"))
	int32 itemAmount;
public:
	FGsSchemaGuildGiftRewardDataList();
	
protected:
	bool operator== (const FGsSchemaGuildGiftRewardDataList& __Other) const
	{
		return FGsSchemaGuildGiftRewardDataList::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
