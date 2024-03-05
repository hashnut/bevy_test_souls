/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedRankEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTitleConditionDataRank.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataRank"))
struct DATACENTER_API FGsSchemaTitleConditionDataRank
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mRankType"))
	RankType mRankType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mRank"))
	int32 mRank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCreatureId"))
	int32 mCreatureId;
public:
	FGsSchemaTitleConditionDataRank();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataRank& __Other) const
	{
		return FGsSchemaTitleConditionDataRank::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataRank::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataRank::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataRankRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataRankRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataRank* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataRank* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataRank>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataRankRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataRankRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataRank final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitleConditionDataRank*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitleConditionDataRank>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mRank, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaTitleConditionDataRank*& OutRow) const
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
		const FGsSchemaTitleConditionDataRank* Temp;
		return !FindRowById(FGsSchemaTitleConditionDataRank().mRank, Temp);
	}
#endif
};
