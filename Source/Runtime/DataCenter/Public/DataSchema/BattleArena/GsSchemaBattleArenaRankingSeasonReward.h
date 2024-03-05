/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleArenaRankingSeasonReward.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsBattleArenaRankingSeasonReward"))
struct DATACENTER_API FGsSchemaBattleArenaRankingSeasonReward
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingGroupData"))
	FGsSchemaRankingGroupData rankingGroupData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardIdSet"))
	TArray<FGsSchemaRewardDataRow> rewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardIdSetShowOnly"))
	TArray<FGsSchemaRewardDataRow> rewardIdSetShowOnly;
public:
	FGsSchemaBattleArenaRankingSeasonReward();
	
protected:
	bool operator== (const FGsSchemaBattleArenaRankingSeasonReward& __Other) const
	{
		return FGsSchemaBattleArenaRankingSeasonReward::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleArenaRankingSeasonReward::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleArenaRankingSeasonReward::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleArenaRankingSeasonRewardRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleArenaRankingSeasonRewardRow() : Super()
	{
	}
	
	const FGsSchemaBattleArenaRankingSeasonReward* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleArenaRankingSeasonReward* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleArenaRankingSeasonReward>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleArenaRankingSeasonRewardRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleArenaRankingSeasonRewardRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleArenaRankingSeasonReward final : public UGsTable
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
		TMap<FName, const FGsSchemaBattleArenaRankingSeasonReward*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattleArenaRankingSeasonReward>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBattleArenaRankingSeasonReward*& OutRow) const
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
		const FGsSchemaBattleArenaRankingSeasonReward* Temp;
		return !FindRowById(FGsSchemaBattleArenaRankingSeasonReward().id, Temp);
	}
#endif
};
