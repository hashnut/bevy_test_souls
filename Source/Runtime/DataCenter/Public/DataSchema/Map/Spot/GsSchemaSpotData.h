/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpotData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spot/BSGsSpotData"))
struct DATACENTER_API FGsSchemaSpotData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId", Tooltip))
	int32 mapId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpotList", Tooltip))
	TArray<FGsSchemaSpotInfo> SpotList;
	
public:
	FGsSchemaSpotData();
	
protected:
	bool operator== (const FGsSchemaSpotData& __Other) const
	{
		return FGsSchemaSpotData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpotData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpotDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpotDataRow() : Super()
	{
	}
	
	const FGsSchemaSpotData* GetRow() const
	{
		const FGsSchemaSpotData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaSpotData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpotDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpotDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpotData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Spot");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSpotData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpotData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSpotData*& OutRow) const
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
		const FGsSchemaSpotData* Temp;
		return !FindRowById(FGsSchemaSpotData().mapId, Temp);
	}
#endif
};
