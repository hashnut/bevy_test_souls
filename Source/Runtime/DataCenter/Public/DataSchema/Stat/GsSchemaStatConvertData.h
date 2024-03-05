/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Stat/GsSchemaStatConvertTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaStatConvertData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsStatConvertData"))
struct DATACENTER_API FGsSchemaStatConvertData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="baseType"))
	FGsStatTypeNameSelector baseType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="convertList"))
	TArray<FGsSchemaStatConvertTable> convertList;
public:
	FGsSchemaStatConvertData();
	
protected:
	bool operator== (const FGsSchemaStatConvertData& __Other) const
	{
		return FGsSchemaStatConvertData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStatConvertData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStatConvertData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStatConvertDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStatConvertDataRow() : Super()
	{
	}
	
	const FGsSchemaStatConvertData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStatConvertData* Row = nullptr;
		return Table->FindRow<FGsSchemaStatConvertData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStatConvertDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStatConvertDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStatConvertData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<FGsStatTypeNameSelector, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Stat");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaStatConvertData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaStatConvertData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->baseType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const FGsStatTypeNameSelector& InId, OUT const FGsSchemaStatConvertData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const FGsStatTypeNameSelector& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaStatConvertData* Temp;
		return !FindRowById(FGsSchemaStatConvertData().baseType, Temp);
	}
#endif
};
