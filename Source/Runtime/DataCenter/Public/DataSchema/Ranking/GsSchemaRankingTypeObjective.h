/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedRankEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRankingTypeObjective.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Ranking/BSGsRankingTypeObjective"))
struct DATACENTER_API FGsSchemaRankingTypeObjective
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="index"))
	int32 index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingType"))
	RankType rankingType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingSearchType"))
	RankSearchType rankingSearchType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingTabName"))
	FText rankingTabName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankUnitName"))
	FText rankUnitName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankUnitName2"))
	FText rankUnitName2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="serverRangeType"))
	RankServerRangeType serverRangeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingCheckCycle"))
	FString rankingCheckCycle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingRewardCycle"))
	FString rankingRewardCycle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingResetCycle"))
	FString rankingResetCycle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxRankCount"))
	int32 maxRankCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useBadge"))
	bool useBadge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="anotherServerList"))
	TArray<FGsSchemaWorldDataRow> anotherServerList;
public:
	FGsSchemaRankingTypeObjective();
	
protected:
	bool operator== (const FGsSchemaRankingTypeObjective& __Other) const
	{
		return FGsSchemaRankingTypeObjective::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRankingTypeObjective::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRankingTypeObjective::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRankingTypeObjectiveRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRankingTypeObjectiveRow() : Super()
	{
	}
	
	const FGsSchemaRankingTypeObjective* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRankingTypeObjective* Row = nullptr;
		return Table->FindRow<FGsSchemaRankingTypeObjective>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRankingTypeObjectiveRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRankingTypeObjectiveRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRankingTypeObjective final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Ranking");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaRankingTypeObjective*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRankingTypeObjective>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->index, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaRankingTypeObjective*& OutRow) const
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
		const FGsSchemaRankingTypeObjective* Temp;
		return !FindRowById(FGsSchemaRankingTypeObjective().index, Temp);
	}
#endif
};
