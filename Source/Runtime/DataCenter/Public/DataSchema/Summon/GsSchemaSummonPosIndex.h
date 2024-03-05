/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSummonPosIndex.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Summon/BSGsSummonPosIndex"))
struct DATACENTER_API FGsSchemaSummonPosIndex
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useSummonCount", Tooltip))
	int32 useSummonCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="arrayPosIndex", Tooltip))
	TArray<int32> arrayPosIndex;
	
public:
	FGsSchemaSummonPosIndex();
	
protected:
	bool operator== (const FGsSchemaSummonPosIndex& __Other) const
	{
		return FGsSchemaSummonPosIndex::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSummonPosIndex::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSummonPosIndex::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSummonPosIndexRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSummonPosIndexRow() : Super()
	{
	}
	
	const FGsSchemaSummonPosIndex* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSummonPosIndex* Row = nullptr;
		return Table->FindRow<FGsSchemaSummonPosIndex>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSummonPosIndexRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSummonPosIndexRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSummonPosIndex final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Summon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSummonPosIndex*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSummonPosIndex>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->useSummonCount, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSummonPosIndex*& OutRow) const
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
		const FGsSchemaSummonPosIndex* Temp;
		return !FindRowById(FGsSchemaSummonPosIndex().useSummonCount, Temp);
	}
#endif
};
