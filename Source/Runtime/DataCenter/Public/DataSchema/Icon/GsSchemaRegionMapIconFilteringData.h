/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Map/GsMapIconType.h"
#include "GsSchemaRegionMapIconFilteringData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsRegionMapIconFilteringData"))
struct DATACENTER_API FGsSchemaRegionMapIconFilteringData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconType"))
	EGsRegionMapIconFiltering _iconType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconImagePath"))
	FSoftObjectPath _iconImagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_textType"))
	FText _textType;
public:
	FGsSchemaRegionMapIconFilteringData();
	
protected:
	bool operator== (const FGsSchemaRegionMapIconFilteringData& __Other) const
	{
		return FGsSchemaRegionMapIconFilteringData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRegionMapIconFilteringData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRegionMapIconFilteringData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRegionMapIconFilteringDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRegionMapIconFilteringDataRow() : Super()
	{
	}
	
	const FGsSchemaRegionMapIconFilteringData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRegionMapIconFilteringData* Row = nullptr;
		return Table->FindRow<FGsSchemaRegionMapIconFilteringData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRegionMapIconFilteringDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRegionMapIconFilteringDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRegionMapIconFilteringData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsRegionMapIconFiltering, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaRegionMapIconFilteringData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRegionMapIconFilteringData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->_iconType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsRegionMapIconFiltering& InId, OUT const FGsSchemaRegionMapIconFilteringData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsRegionMapIconFiltering& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaRegionMapIconFilteringData* Temp;
		return !FindRowById(FGsSchemaRegionMapIconFilteringData()._iconType, Temp);
	}
#endif
};
