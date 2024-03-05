/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/Area/GsSchemaAreaChannelInfo.h"
#include "DataSchema/Map/Area/GsSchemaAreaEnv.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAreaData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Area/BSGsAreaData"))
struct DATACENTER_API FGsSchemaAreaData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="continentId", Tooltip))
	int32 continentId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelName", Tooltip))
	FString levelName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="channel", Tooltip))
	FGsSchemaAreaChannelInfo channel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaAreaEnv areaEnv;
	
public:
	FGsSchemaAreaData();
	
protected:
	bool operator== (const FGsSchemaAreaData& __Other) const
	{
		return FGsSchemaAreaData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAreaData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAreaData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAreaDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAreaDataRow() : Super()
	{
	}
	
	const FGsSchemaAreaData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAreaData* Row = nullptr;
		return Table->FindRow<FGsSchemaAreaData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAreaDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAreaDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAreaData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Area");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAreaData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAreaData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaAreaData*& OutRow) const
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
		const FGsSchemaAreaData* Temp;
		return !FindRowById(FGsSchemaAreaData().id, Temp);
	}
#endif
};
