/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/FieldBoss/GsSchemaFieldBossMutantApear.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBossEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/FieldBoss/GsSchemaFieldBossRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/EventAction/GsSchemaMapEventActionImmediate.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFieldBossData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/FieldBoss/BSGsFieldBossData"))
struct DATACENTER_API FGsSchemaFieldBossData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossId"))
	int32 bossId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bossType"))
	BossType bossType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossNpcId"))
	FGsSchemaNpcDataRow bossNpcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nickName"))
	FText nickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fieldBossImg"))
	FSoftObjectPath fieldBossImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portraitImg"))
	FSoftObjectPath portraitImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldMapMark"))
	bool worldMapMark;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnGroupIdList"))
	TArray<int64> spawnGroupIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="WarpSpotId"))
	int32 WarpSpotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnType"))
	BossSpawnType spawnType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnChannelType"))
	BossSpawnChannel spawnChannelType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossRewardIdList"))
	TArray<FGsSchemaFieldBossRewardDataRow> bossRewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionMin"))
	int64 contributionMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startDateTime"))
	FString startDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endDateTime"))
	FString endDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnIntervalM"))
	int32 spawnIntervalM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnIntervalDeviationM"))
	int32 spawnIntervalDeviationM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="makeCron"))
	FString makeCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnProbability"))
	int32 spawnProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="despawnTimeM"))
	int32 despawnTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mutantApear"))
	FGsSchemaFieldBossMutantApear mutantApear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cameraRot"))
	FRotator cameraRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cameraMinDist"))
	float cameraMinDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cameraMaxDist"))
	float cameraMaxDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tickerEventSpawnId"))
	FGsSchemaMapEventActionImmediateRow tickerEventSpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tickerEventDieId"))
	FGsSchemaMapEventActionImmediateRow tickerEventDieId;
public:
	FGsSchemaFieldBossData();
	
protected:
	bool operator== (const FGsSchemaFieldBossData& __Other) const
	{
		return FGsSchemaFieldBossData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFieldBossData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFieldBossData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFieldBossDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFieldBossDataRow() : Super()
	{
	}
	
	const FGsSchemaFieldBossData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFieldBossData* Row = nullptr;
		return Table->FindRow<FGsSchemaFieldBossData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFieldBossDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFieldBossDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFieldBossData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/FieldBoss");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFieldBossData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFieldBossData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->bossId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaFieldBossData*& OutRow) const
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
		const FGsSchemaFieldBossData* Temp;
		return !FindRowById(FGsSchemaFieldBossData().bossId, Temp);
	}
#endif
};
