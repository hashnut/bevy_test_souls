/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Market/GsSchemaMarketSubCategoryData.h"
#include "GsSchemaMarketCategoryData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Market/BSGsMarketCategoryData"))
struct DATACENTER_API FGsSchemaMarketCategoryData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="category", Tooltip))
	EGsMarketUICategory category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryText", Tooltip))
	FText categoryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subCategoryList", Tooltip))
	TArray<FGsSchemaMarketSubCategoryData> subCategoryList;
public:
	FGsSchemaMarketCategoryData();
	
protected:
	bool operator== (const FGsSchemaMarketCategoryData& __Other) const
	{
		return FGsSchemaMarketCategoryData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMarketCategoryData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMarketCategoryData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMarketCategoryDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMarketCategoryDataRow() : Super()
	{
	}
	
	const FGsSchemaMarketCategoryData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMarketCategoryData* Row = nullptr;
		return Table->FindRow<FGsSchemaMarketCategoryData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMarketCategoryDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMarketCategoryDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMarketCategoryData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsMarketUICategory, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Market");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMarketCategoryData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMarketCategoryData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->category, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsMarketUICategory& InId, OUT const FGsSchemaMarketCategoryData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsMarketUICategory& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaMarketCategoryData* Temp;
		return !FindRowById(FGsSchemaMarketCategoryData().category, Temp);
	}
#endif
};
