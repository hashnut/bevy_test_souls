/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaTierData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleArenaTierSeasonReward.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsBattleArenaTierSeasonReward"))
struct DATACENTER_API FGsSchemaBattleArenaTierSeasonReward
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tierDataId"))
	FGsSchemaBattleArenaTierDataRow tierDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardIdSet"))
	TArray<FGsSchemaRewardDataRow> rewardIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardIdSetShowOnly"))
	TArray<FGsSchemaRewardDataRow> rewardIdSetShowOnly;
public:
	FGsSchemaBattleArenaTierSeasonReward();
	
protected:
	bool operator== (const FGsSchemaBattleArenaTierSeasonReward& __Other) const
	{
		return FGsSchemaBattleArenaTierSeasonReward::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleArenaTierSeasonReward::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleArenaTierSeasonReward::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleArenaTierSeasonRewardRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleArenaTierSeasonRewardRow() : Super()
	{
	}
	
	const FGsSchemaBattleArenaTierSeasonReward* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleArenaTierSeasonReward* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleArenaTierSeasonReward>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleArenaTierSeasonRewardRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleArenaTierSeasonRewardRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleArenaTierSeasonReward final : public UGsTable
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
		TMap<FName, const FGsSchemaBattleArenaTierSeasonReward*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattleArenaTierSeasonReward>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBattleArenaTierSeasonReward*& OutRow) const
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
		const FGsSchemaBattleArenaTierSeasonReward* Temp;
		return !FindRowById(FGsSchemaBattleArenaTierSeasonReward().id, Temp);
	}
#endif
};
