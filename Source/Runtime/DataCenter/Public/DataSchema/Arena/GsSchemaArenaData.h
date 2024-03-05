/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaArenaData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Arena/BSGsArenaData"))
struct DATACENTER_API FGsSchemaArenaData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="enterSpotIdList"))
	TArray<int32> enterSpotIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exitSpotIdList"))
	TArray<int32> exitSpotIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="arenaTimeSec"))
	int32 arenaTimeSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="arenaCountSec"))
	int32 arenaCountSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="arenaRequestTime"))
	int32 arenaRequestTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="arenaWinSound"))
	FGsSchemaSoundResDataRow arenaWinSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="arenaLoseSound"))
	FGsSchemaSoundResDataRow arenaLoseSound;
public:
	FGsSchemaArenaData();
	
protected:
	bool operator== (const FGsSchemaArenaData& __Other) const
	{
		return FGsSchemaArenaData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaArenaData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaArenaData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaArenaDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaArenaDataRow() : Super()
	{
	}
	
	const FGsSchemaArenaData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaArenaData* Row = nullptr;
		return Table->FindRow<FGsSchemaArenaData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaArenaDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaArenaDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableArenaData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Arena");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaArenaData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaArenaData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaArenaData*& OutRow) const
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
		const FGsSchemaArenaData* Temp;
		return !FindRowById(FGsSchemaArenaData().mapId, Temp);
	}
#endif
};
