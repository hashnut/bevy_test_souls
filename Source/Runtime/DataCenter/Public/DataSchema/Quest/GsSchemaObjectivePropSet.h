/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaObjectivePropSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsObjectivePropSet"))
struct DATACENTER_API FGsSchemaObjectivePropSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="propId"))
	FGsSchemaPropDataRow propId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="count"))
	int32 count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropRate"))
	float dropRate;
public:
	FGsSchemaObjectivePropSet();
	
protected:
	bool operator== (const FGsSchemaObjectivePropSet& __Other) const
	{
		return FGsSchemaObjectivePropSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaObjectivePropSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaObjectivePropSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaObjectivePropSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaObjectivePropSetRow() : Super()
	{
	}
	
	const FGsSchemaObjectivePropSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaObjectivePropSet* Row = nullptr;
		return Table->FindRow<FGsSchemaObjectivePropSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaObjectivePropSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaObjectivePropSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableObjectivePropSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaObjectivePropSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaObjectivePropSet>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaObjectivePropSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaObjectivePropSet* Temp;
		return !FindRowById(FGsSchemaObjectivePropSet().id, Temp);
	}
#endif
};
