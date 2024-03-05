/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaObjectiveWarpToInstanceFieldSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsObjectiveWarpToInstanceFieldSet"))
struct DATACENTER_API FGsSchemaObjectiveWarpToInstanceFieldSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Id"))
	int64 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pos"))
	FGsSchemaQuestObjectiveAreaPos pos;
public:
	FGsSchemaObjectiveWarpToInstanceFieldSet();
	
protected:
	bool operator== (const FGsSchemaObjectiveWarpToInstanceFieldSet& __Other) const
	{
		return FGsSchemaObjectiveWarpToInstanceFieldSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaObjectiveWarpToInstanceFieldSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaObjectiveWarpToInstanceFieldSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaObjectiveWarpToInstanceFieldSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaObjectiveWarpToInstanceFieldSetRow() : Super()
	{
	}
	
	const FGsSchemaObjectiveWarpToInstanceFieldSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaObjectiveWarpToInstanceFieldSet* Row = nullptr;
		return Table->FindRow<FGsSchemaObjectiveWarpToInstanceFieldSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaObjectiveWarpToInstanceFieldSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaObjectiveWarpToInstanceFieldSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableObjectiveWarpToInstanceFieldSet final : public UGsTable
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
		TMap<FName, const FGsSchemaObjectiveWarpToInstanceFieldSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaObjectiveWarpToInstanceFieldSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->Id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaObjectiveWarpToInstanceFieldSet*& OutRow) const
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
		const FGsSchemaObjectiveWarpToInstanceFieldSet* Temp;
		return !FindRowById(FGsSchemaObjectiveWarpToInstanceFieldSet().Id, Temp);
	}
#endif
};
