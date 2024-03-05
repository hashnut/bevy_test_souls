/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMapDungeonData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsWorldMapDungeonData"))
struct DATACENTER_API FGsSchemaWorldMapDungeonData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="regionMapId"))
	TArray<int32> regionMapId;
public:
	FGsSchemaWorldMapDungeonData();
	
protected:
	bool operator== (const FGsSchemaWorldMapDungeonData& __Other) const
	{
		return FGsSchemaWorldMapDungeonData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldMapDungeonData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldMapDungeonData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldMapDungeonDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldMapDungeonDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldMapDungeonData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldMapDungeonData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldMapDungeonData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldMapDungeonDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldMapDungeonDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldMapDungeonData final : public UGsTable
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
		TMap<FName, const FGsSchemaWorldMapDungeonData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldMapDungeonData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->groupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldMapDungeonData*& OutRow) const
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
		const FGsSchemaWorldMapDungeonData* Temp;
		return !FindRowById(FGsSchemaWorldMapDungeonData().groupId, Temp);
	}
#endif
};
