/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryZoneShape.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "GsSchemaClientSpawnData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsClientSpawnData"))
struct DATACENTER_API FGsSchemaClientSpawnData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Territory zone unique id"))
	int32 SpawnGroupId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="byClient", Tooltip))
	bool byClient;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="byDefault", Tooltip))
	bool byDefault;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaMapDataRow MapId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Region", Tooltip))
	FGsSchemaTerritoryZoneShape Region;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpawnInfoList", Tooltip))
	TArray<FGsSchemaSpawnElement> SpawnInfoList;
	
public:
	FGsSchemaClientSpawnData();
	
protected:
	bool operator== (const FGsSchemaClientSpawnData& __Other) const
	{
		return FGsSchemaClientSpawnData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaClientSpawnData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaClientSpawnData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaClientSpawnDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaClientSpawnDataRow() : Super()
	{
	}
	
	const FGsSchemaClientSpawnData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaClientSpawnData* Row = nullptr;
		return Table->FindRow<FGsSchemaClientSpawnData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaClientSpawnDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaClientSpawnDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableClientSpawnData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Spawn");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaClientSpawnData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaClientSpawnData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->SpawnGroupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaClientSpawnData*& OutRow) const
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
		const FGsSchemaClientSpawnData* Temp;
		return !FindRowById(FGsSchemaClientSpawnData().SpawnGroupId, Temp);
	}
#endif
};
