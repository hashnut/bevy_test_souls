/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaRandomSpawnGroupInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRandomGroupSpawn.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsRandomGroupSpawn"))
struct DATACENTER_API FGsSchemaRandomGroupSpawn
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomGroupSpawnId"))
	int32 randomGroupSpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomType"))
	RandomGroupSpawnType randomType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="selectCount"))
	int32 selectCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomSpawnGroupInfoList"))
	TArray<FGsSchemaRandomSpawnGroupInfo> randomSpawnGroupInfoList;
public:
	FGsSchemaRandomGroupSpawn();
	
protected:
	bool operator== (const FGsSchemaRandomGroupSpawn& __Other) const
	{
		return FGsSchemaRandomGroupSpawn::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRandomGroupSpawn::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRandomGroupSpawn::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRandomGroupSpawnRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRandomGroupSpawnRow() : Super()
	{
	}
	
	const FGsSchemaRandomGroupSpawn* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRandomGroupSpawn* Row = nullptr;
		return Table->FindRow<FGsSchemaRandomGroupSpawn>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRandomGroupSpawnRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRandomGroupSpawnRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRandomGroupSpawn final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaRandomGroupSpawn*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRandomGroupSpawn>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->randomGroupSpawnId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaRandomGroupSpawn*& OutRow) const
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
		const FGsSchemaRandomGroupSpawn* Temp;
		return !FindRowById(FGsSchemaRandomGroupSpawn().randomGroupSpawnId, Temp);
	}
#endif
};
