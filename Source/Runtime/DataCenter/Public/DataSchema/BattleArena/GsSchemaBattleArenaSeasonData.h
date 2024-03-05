/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaRankingSeasonReward.h"
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaTierSeasonReward.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleArenaSeasonData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsBattleArenaSeasonData"))
struct DATACENTER_API FGsSchemaBattleArenaSeasonData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="seasonNumber"))
	int32 seasonNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="beginDate"))
	FString beginDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endDate"))
	FString endDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankRewardMailTypeId"))
	FGsSchemaMailTypeRow rankRewardMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tierRewardMailTypeId"))
	FGsSchemaMailTypeRow tierRewardMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankRewardIdSet"))
	TArray<FGsSchemaBattleArenaRankingSeasonRewardRow> rankRewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tierRewardIdSet"))
	TArray<FGsSchemaBattleArenaTierSeasonRewardRow> tierRewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playRewardLimitCount"))
	int32 playRewardLimitCount;
public:
	FGsSchemaBattleArenaSeasonData();
	
protected:
	bool operator== (const FGsSchemaBattleArenaSeasonData& __Other) const
	{
		return FGsSchemaBattleArenaSeasonData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleArenaSeasonData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleArenaSeasonData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleArenaSeasonDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleArenaSeasonDataRow() : Super()
	{
	}
	
	const FGsSchemaBattleArenaSeasonData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleArenaSeasonData* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleArenaSeasonData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleArenaSeasonDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleArenaSeasonDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleArenaSeasonData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattleArena");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBattleArenaSeasonData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattleArenaSeasonData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->seasonNumber, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaBattleArenaSeasonData*& OutRow) const
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
		const FGsSchemaBattleArenaSeasonData* Temp;
		return !FindRowById(FGsSchemaBattleArenaSeasonData().seasonNumber, Temp);
	}
#endif
};
