/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaObjectiveGotoPosSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsObjectiveGotoPosSet"))
struct DATACENTER_API FGsSchemaObjectiveGotoPosSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="range"))
	float range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spotId"))
	int32 spotId;
public:
	FGsSchemaObjectiveGotoPosSet();
	
protected:
	bool operator== (const FGsSchemaObjectiveGotoPosSet& __Other) const
	{
		return FGsSchemaObjectiveGotoPosSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaObjectiveGotoPosSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaObjectiveGotoPosSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaObjectiveGotoPosSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaObjectiveGotoPosSetRow() : Super()
	{
	}
	
	const FGsSchemaObjectiveGotoPosSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaObjectiveGotoPosSet* Row = nullptr;
		return Table->FindRow<FGsSchemaObjectiveGotoPosSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaObjectiveGotoPosSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaObjectiveGotoPosSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableObjectiveGotoPosSet final : public UGsTable
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
		TMap<FName, const FGsSchemaObjectiveGotoPosSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaObjectiveGotoPosSet>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaObjectiveGotoPosSet*& OutRow) const
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
		const FGsSchemaObjectiveGotoPosSet* Temp;
		return !FindRowById(FGsSchemaObjectiveGotoPosSet().id, Temp);
	}
#endif
};
