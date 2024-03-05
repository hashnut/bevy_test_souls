/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/EventAction/GsSchemaMapEventActionDialog.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapEventActionImmediate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/EventAction/BSGsMapEventActionImmediate"))
struct DATACENTER_API FGsSchemaMapEventActionImmediate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="data"))
	FGsSchemaMapEventActionDialogRow data;
public:
	FGsSchemaMapEventActionImmediate();
	
protected:
	bool operator== (const FGsSchemaMapEventActionImmediate& __Other) const
	{
		return FGsSchemaMapEventActionImmediate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapEventActionImmediate::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapEventActionImmediate::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapEventActionImmediateRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapEventActionImmediateRow() : Super()
	{
	}
	
	const FGsSchemaMapEventActionImmediate* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapEventActionImmediate* Row = nullptr;
		return Table->FindRow<FGsSchemaMapEventActionImmediate>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapEventActionImmediateRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapEventActionImmediateRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapEventActionImmediate final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/EventAction");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMapEventActionImmediate*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMapEventActionImmediate>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMapEventActionImmediate*& OutRow) const
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
		const FGsSchemaMapEventActionImmediate* Temp;
		return !FindRowById(FGsSchemaMapEventActionImmediate().id, Temp);
	}
#endif
};
