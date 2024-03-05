/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/GsSchemaInvadeRewardInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaRandomGroupSpawn.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInvadeMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInvadeMapData"))
struct DATACENTER_API FGsSchemaInvadeMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portalUserCount"))
	int32 portalUserCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portalSpawnCount"))
	int32 portalSpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portalSpawnProbability"))
	int32 portalSpawnProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portalSpotList"))
	TArray<int64> portalSpotList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enterAllowLowLevel"))
	int32 enterAllowLowLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enterAllowHighLevel"))
	int32 enterAllowHighLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadePassivityIdList"))
	TArray<FGsSchemaPassivitySetRow> invadePassivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadePassivityIdListIndexEnv"))
	int32 invadePassivityIdListIndexEnv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="defensePassivityIdList"))
	TArray<FGsSchemaPassivitySetRow> defensePassivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="defensePassivityIdListIndexEnv"))
	int32 defensePassivityIdListIndexEnv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeGuardianSpawnGroupId"))
	FGsSchemaSpawnDataRow invadeGuardianSpawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeMonsterSpawnCheckStartTimeS"))
	int32 invadeMonsterSpawnCheckStartTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeMonsterSpawnCheckIntervalTimeS"))
	int32 invadeMonsterSpawnCheckIntervalTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeMonsterSpawnSuccessRate"))
	int32 invadeMonsterSpawnSuccessRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="InvadeMonsterRandomGroupSpawnId"))
	FGsSchemaRandomGroupSpawnRow InvadeMonsterRandomGroupSpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeTreasureBoxSpawnCheckStartTimeS"))
	int32 invadeTreasureBoxSpawnCheckStartTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeTreasureBoxSpawnCheckIntervalTimeS"))
	int32 invadeTreasureBoxSpawnCheckIntervalTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeTreasureBoxRandomGroupSpawnId"))
	FGsSchemaRandomGroupSpawnRow invadeTreasureBoxRandomGroupSpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeWagonRandomGroupSpawnId"))
	FGsSchemaRandomGroupSpawnRow invadeWagonRandomGroupSpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeGuardianPassivityId"))
	FGsSchemaPassivitySetRow invadeGuardianPassivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeGuardianDeathPenaltyAbnormalityId"))
	FGsSchemaAbnormalitySetRow invadeGuardianDeathPenaltyAbnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeWagonRandomTableTotalCount"))
	int32 invadeWagonRandomTableTotalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeWagonRandomTableSpawnCount"))
	int32 invadeWagonRandomTableSpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeWagonBonusRewardList"))
	TArray<FGsSchemaInvadeRewardInfo> invadeWagonBonusRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeGuardianBonusRewardList"))
	TArray<FGsSchemaInvadeRewardInfo> invadeGuardianBonusRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invadeMonsterBonusRewardList"))
	TArray<FGsSchemaInvadeRewardInfo> invadeMonsterBonusRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxInvaderCount"))
	int32 maxInvaderCount;
public:
	FGsSchemaInvadeMapData();
	
protected:
	bool operator== (const FGsSchemaInvadeMapData& __Other) const
	{
		return FGsSchemaInvadeMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInvadeMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInvadeMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInvadeMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInvadeMapDataRow() : Super()
	{
	}
	
	const FGsSchemaInvadeMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInvadeMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaInvadeMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInvadeMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInvadeMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInvadeMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
};
