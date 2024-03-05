/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/WorldBoss/GsSchemaRewardByRankData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldBossData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/WorldBoss/BSGsWorldBossData"))
struct DATACENTER_API FGsSchemaWorldBossData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossNpcId"))
	FGsSchemaNpcDataRow bossNpcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nickName"))
	FText nickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossImg"))
	FSoftObjectPath bossImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portraitImg"))
	FSoftObjectPath portraitImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="warpSpotIdList"))
	TArray<int32> warpSpotIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userMax"))
	int32 userMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userMinRecommend"))
	int32 userMinRecommend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationTimeMinute"))
	int32 durationTimeMinute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="basicRewardIdList"))
	TArray<FGsSchemaRewardDataRow> basicRewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankRewardIdList"))
	TArray<FGsSchemaRewardByRankDataRow> rankRewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="lastHitRewardIdList"))
	TArray<FGsSchemaRewardDataRow> lastHitRewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="consolationRewardIdList"))
	TArray<FGsSchemaRewardDataRow> consolationRewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionMin"))
	int32 contributionMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startDateTime"))
	FString startDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endDateTime"))
	FString endDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnTimeCron"))
	FString spawnTimeCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="unlockContentsId"))
	FGsSchemaUnlockContentsRow unlockContentsId;
public:
	FGsSchemaWorldBossData();
	
protected:
	bool operator== (const FGsSchemaWorldBossData& __Other) const
	{
		return FGsSchemaWorldBossData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldBossData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldBossData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldBossDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldBossDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldBossData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldBossData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldBossData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldBossDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldBossDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldBossData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("WorldBoss");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWorldBossData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldBossData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldBossData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaWorldBossData* Temp;
		return !FindRowById(FGsSchemaWorldBossData().id, Temp);
	}
#endif
};
