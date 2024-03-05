/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSummonCostumeAppearResPath.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Summon/BSGsSummonCostumeAppearResPath"))
struct DATACENTER_API FGsSchemaSummonCostumeAppearResPath
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeId"))
	int32 costumeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="appearSequenceId"))
	TArray<int32> appearSequenceId;
public:
	FGsSchemaSummonCostumeAppearResPath();
	
protected:
	bool operator== (const FGsSchemaSummonCostumeAppearResPath& __Other) const
	{
		return FGsSchemaSummonCostumeAppearResPath::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSummonCostumeAppearResPath::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSummonCostumeAppearResPath::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSummonCostumeAppearResPathRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSummonCostumeAppearResPathRow() : Super()
	{
	}
	
	const FGsSchemaSummonCostumeAppearResPath* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSummonCostumeAppearResPath* Row = nullptr;
		return Table->FindRow<FGsSchemaSummonCostumeAppearResPath>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSummonCostumeAppearResPathRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSummonCostumeAppearResPathRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSummonCostumeAppearResPath final : public UGsTable
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
		TMap<FName, const FGsSchemaSummonCostumeAppearResPath*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSummonCostumeAppearResPath>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->costumeId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSummonCostumeAppearResPath*& OutRow) const
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
		const FGsSchemaSummonCostumeAppearResPath* Temp;
		return !FindRowById(FGsSchemaSummonCostumeAppearResPath().costumeId, Temp);
	}
#endif
};
