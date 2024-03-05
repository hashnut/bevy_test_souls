/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContinentalMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsContinentalMapData"))
struct DATACENTER_API FGsSchemaContinentalMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="continentalBP"))
	FSoftObjectPath continentalBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapStringId"))
	FText mapStringId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="timeZoneStringId"))
	FText timeZoneStringId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="continentalLoopBP"))
	FSoftObjectPath continentalLoopBP;
public:
	FGsSchemaContinentalMapData();
	
protected:
	bool operator== (const FGsSchemaContinentalMapData& __Other) const
	{
		return FGsSchemaContinentalMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContinentalMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContinentalMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContinentalMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContinentalMapDataRow() : Super()
	{
	}
	
	const FGsSchemaContinentalMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContinentalMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaContinentalMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContinentalMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContinentalMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContinentalMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/WorldMap");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContinentalMapData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContinentalMapData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContinentalMapData*& OutRow) const
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
		const FGsSchemaContinentalMapData* Temp;
		return !FindRowById(FGsSchemaContinentalMapData().id, Temp);
	}
#endif
};
