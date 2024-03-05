/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Line/GsSchemaLineElement.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLineData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Line/BSGsLineData"))
struct DATACENTER_API FGsSchemaLineData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MapId"))
	int64 MapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="LineList"))
	TArray<FGsSchemaLineElement> LineList;
public:
	FGsSchemaLineData();
	
protected:
	bool operator== (const FGsSchemaLineData& __Other) const
	{
		return FGsSchemaLineData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLineData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLineData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLineDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLineDataRow() : Super()
	{
	}
	
	const FGsSchemaLineData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLineData* Row = nullptr;
		return Table->FindRow<FGsSchemaLineData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLineDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLineDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLineData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Line");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaLineData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaLineData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->MapId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaLineData*& OutRow) const
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
		const FGsSchemaLineData* Temp;
		return !FindRowById(FGsSchemaLineData().MapId, Temp);
	}
#endif
};
