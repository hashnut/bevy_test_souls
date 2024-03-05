/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaTerritoryZoneShape.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpawnData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsSpawnData"))
struct DATACENTER_API FGsSchemaSpawnData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Territory zone unique id", DisplayName="SpawnGroupId"))
	int32 SpawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpawnGroupType"))
	NpcSpawnGroupType SpawnGroupType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int64 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="byClient"))
	bool byClient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="byDefault"))
	bool byDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="MapId"))
	FGsSchemaMapDataRow MapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Region"))
	FGsSchemaTerritoryZoneShape Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpawnInfoList"))
	TArray<FGsSchemaSpawnElement> SpawnInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomSpawnMinDistance"))
	int32 randomSpawnMinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="lineId"))
	int64 lineId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupLineMove"))
	bool groupLineMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomSpotList"))
	TArray<int64> randomSpotList;
public:
	FGsSchemaSpawnData();
	
protected:
	bool operator== (const FGsSchemaSpawnData& __Other) const
	{
		return FGsSchemaSpawnData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpawnData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpawnDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpawnDataRow() : Super()
	{
	}
	
	const FGsSchemaSpawnData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpawnData* Row = nullptr;
		return Table->FindRow<FGsSchemaSpawnData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpawnDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpawnDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpawnData final : public UGsTable
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
		TMap<FName, const FGsSchemaSpawnData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpawnData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSpawnData*& OutRow) const
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
		const FGsSchemaSpawnData* Temp;
		return !FindRowById(FGsSchemaSpawnData().SpawnGroupId, Temp);
	}
#endif
};
