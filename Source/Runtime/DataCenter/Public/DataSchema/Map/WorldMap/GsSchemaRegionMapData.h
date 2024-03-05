/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRegionMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsRegionMapData"))
struct DATACENTER_API FGsSchemaRegionMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="regionImagePath", Tooltip))
	FSoftObjectPath regionImagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="realMapSize", Tooltip))
	FVector2D realMapSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="regionMapSize", Tooltip))
	FVector2D regionMapSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UIShiftMapPos", Tooltip))
	FVector2D UIShiftMapPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="regionMapShow", Tooltip))
	bool regionMapShow;
public:
	FGsSchemaRegionMapData();
	
protected:
	bool operator== (const FGsSchemaRegionMapData& __Other) const
	{
		return FGsSchemaRegionMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRegionMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRegionMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRegionMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRegionMapDataRow() : Super()
	{
	}
	
	const FGsSchemaRegionMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRegionMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaRegionMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRegionMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRegionMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRegionMapData final : public UGsTable
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
		TMap<FName, const FGsSchemaRegionMapData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRegionMapData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaRegionMapData*& OutRow) const
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
		const FGsSchemaRegionMapData* Temp;
		return !FindRowById(FGsSchemaRegionMapData().id, Temp);
	}
#endif
};
