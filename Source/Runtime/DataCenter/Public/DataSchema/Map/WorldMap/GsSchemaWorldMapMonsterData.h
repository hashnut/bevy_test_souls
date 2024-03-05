/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMapMonsterData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsWorldMapMonsterData"))
struct DATACENTER_API FGsSchemaWorldMapMonsterData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsters"))
	TArray<FGsSchemaWorldMapMonsterElementDataRow> monsters;
public:
	FGsSchemaWorldMapMonsterData();
	
protected:
	bool operator== (const FGsSchemaWorldMapMonsterData& __Other) const
	{
		return FGsSchemaWorldMapMonsterData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldMapMonsterData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldMapMonsterData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldMapMonsterDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldMapMonsterDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldMapMonsterData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldMapMonsterData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldMapMonsterData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldMapMonsterDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldMapMonsterDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldMapMonsterData final : public UGsTable
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
		TMap<FName, const FGsSchemaWorldMapMonsterData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldMapMonsterData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldMapMonsterData*& OutRow) const
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
		const FGsSchemaWorldMapMonsterData* Temp;
		return !FindRowById(FGsSchemaWorldMapMonsterData().mapId, Temp);
	}
#endif
};
