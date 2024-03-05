/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsItem.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMapSpoilsData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsWorldMapSpoilsData"))
struct DATACENTER_API FGsSchemaWorldMapSpoilsData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipSpoils"))
	TArray<FGsSchemaWorldMapSpoilsItem> equipSpoils;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="etcSpoils"))
	TArray<FGsSchemaWorldMapSpoilsItem> etcSpoils;
public:
	FGsSchemaWorldMapSpoilsData();
	
protected:
	bool operator== (const FGsSchemaWorldMapSpoilsData& __Other) const
	{
		return FGsSchemaWorldMapSpoilsData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldMapSpoilsData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldMapSpoilsData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldMapSpoilsDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldMapSpoilsDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldMapSpoilsData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldMapSpoilsData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldMapSpoilsData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldMapSpoilsDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldMapSpoilsDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldMapSpoilsData final : public UGsTable
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
		TMap<FName, const FGsSchemaWorldMapSpoilsData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldMapSpoilsData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mapId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldMapSpoilsData*& OutRow) const
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
		const FGsSchemaWorldMapSpoilsData* Temp;
		return !FindRowById(FGsSchemaWorldMapSpoilsData().mapId, Temp);
	}
#endif
};
