/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFenceData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Fence/BSGsFenceData"))
struct DATACENTER_API FGsSchemaFenceData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId", Tooltip))
	int32 mapId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="FenceList", Tooltip))
	TArray<FGsSchemaFenceElement> FenceList;
	
public:
	FGsSchemaFenceData();
	
protected:
	bool operator== (const FGsSchemaFenceData& __Other) const
	{
		return FGsSchemaFenceData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFenceData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFenceDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFenceDataRow() : Super()
	{
	}
	
	const FGsSchemaFenceData* GetRow() const
	{
		const FGsSchemaFenceData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaFenceData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFenceDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFenceDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFenceData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Fence");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFenceData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFenceData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFenceData*& OutRow) const
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
		const FGsSchemaFenceData* Temp;
		return !FindRowById(FGsSchemaFenceData().mapId, Temp);
	}
#endif
};
