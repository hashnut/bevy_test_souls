/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaRewardPointData.h"
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaSeasonData.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailType.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/EventAction/GsSchemaMapEventActionImmediate.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildWarConfigData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsGuildWarConfigData"))
struct DATACENTER_API FGsSchemaGuildWarConfigData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungoneGroupId"))
	FGsSchemaDungeonGroupDataRow dungoneGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapDataId"))
	FGsSchemaMapDataRow mapDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="seasonDataId"))
	FGsSchemaBattleArenaSeasonDataRow seasonDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitGuildLevel"))
	int32 limitGuildLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitMinApplyGrade"))
	GuildMemberGradeType limitMinApplyGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitMinMemberCount"))
	int32 limitMinMemberCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitMaxMemberCount"))
	int32 limitMaxMemberCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitMinRewardPoint"))
	int32 limitMinRewardPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userKillPoint"))
	int32 userKillPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userAssitPoint"))
	int32 userAssitPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcPointDataSet"))
	TArray<FGsSchemaBattleArenaRewardPointData> npcPointDataSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="redSpotId"))
	int32 redSpotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="blueSpotId"))
	int32 blueSpotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openingCycling"))
	FString openingCycling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterDelayTimeApplyEnable"))
	int32 afterDelayTimeApplyEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterDelayTimeReady"))
	int32 afterDelayTimeReady;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterDelayTimePlayWait"))
	int32 afterDelayTimePlayWait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterDelayTimePlay"))
	int32 afterDelayTimePlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterDelayTimeEnd"))
	int32 afterDelayTimeEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterDelayTimeExit"))
	int32 afterDelayTimeExit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="winMailTypeId"))
	FGsSchemaMailTypeRow winMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="winRewardIdSet"))
	TArray<FGsSchemaRewardDataRow> winRewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="defeatMailTypeId"))
	FGsSchemaMailTypeRow defeatMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="defeatRewardIdSet"))
	TArray<FGsSchemaRewardDataRow> defeatRewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="drawMailTypeId"))
	FGsSchemaMailTypeRow drawMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="drawRewardIdSet"))
	TArray<FGsSchemaRewardDataRow> drawRewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossNpcId"))
	FGsSchemaNpcDataRow bossNpcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="coinId"))
	FGsSchemaItemCommonRow coinId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="basicMatchingPoint"))
	int32 basicMatchingPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="matchingPointVariable1"))
	int32 matchingPointVariable1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="matchingPointVariable2"))
	int32 matchingPointVariable2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="matchingPointWin"))
	int32 matchingPointWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="matchingPointDraw"))
	int32 matchingPointDraw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="matchingPointNoBattleWin"))
	int32 matchingPointNoBattleWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="matchingPointLose"))
	int32 matchingPointLose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="openEventTickerId"))
	FGsSchemaMapEventActionImmediateRow openEventTickerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openEventChatTickerId"))
	FGsSchemaMapEventActionImmediateRow openEventChatTickerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mainInterWorldServerId"))
	FGsSchemaWorldDataRow mainInterWorldServerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="playTimeText"))
	FText playTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="entranceCountdownSet"))
	TArray<int32> entranceCountdownSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endPlayCountdownSet"))
	TArray<int32> endPlayCountdownSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusRankingPoint"))
	int32 bonusRankingPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gameOverScore"))
	int32 gameOverScore;
public:
	FGsSchemaGuildWarConfigData();
	
protected:
	bool operator== (const FGsSchemaGuildWarConfigData& __Other) const
	{
		return FGsSchemaGuildWarConfigData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildWarConfigData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildWarConfigData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildWarConfigDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildWarConfigDataRow() : Super()
	{
	}
	
	const FGsSchemaGuildWarConfigData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildWarConfigData* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildWarConfigData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildWarConfigDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildWarConfigDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildWarConfigData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattleArena");
	}
	
};
