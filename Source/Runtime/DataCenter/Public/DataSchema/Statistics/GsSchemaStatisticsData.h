/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedStatisticsEnum.h"
#include "GsSchemaStatisticsData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Statistics/BSGsStatisticsData"))
struct DATACENTER_API FGsSchemaStatisticsData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statistics"))
	StatisticsType statistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabId"))
	int32 tabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subCategory"))
	EGsStatisticsSubCategory subCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subCategoryNameText"))
	FText subCategoryNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useDailyGraph"))
	bool useDailyGraph;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="valueType"))
	EGsStatisticsValueType valueType;
public:
	FGsSchemaStatisticsData();
	
protected:
	bool operator== (const FGsSchemaStatisticsData& __Other) const
	{
		return FGsSchemaStatisticsData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStatisticsData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStatisticsData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStatisticsDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStatisticsDataRow() : Super()
	{
	}
	
	const FGsSchemaStatisticsData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStatisticsData* Row = nullptr;
		return Table->FindRow<FGsSchemaStatisticsData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStatisticsDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStatisticsDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStatisticsData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Statistics");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaStatisticsData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaStatisticsData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaStatisticsData*& OutRow) const
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
		const FGsSchemaStatisticsData* Temp;
		return !FindRowById(FGsSchemaStatisticsData().id, Temp);
	}
#endif
};
