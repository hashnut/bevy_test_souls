/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Scan/GsScanIconType.h"
#include "GsSchemaScanIconData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsScanIconData"))
struct DATACENTER_API FGsSchemaScanIconData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconType"))
	EGsScanIconType _iconType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconImagePath"))
	FSoftObjectPath _iconImagePath;
public:
	FGsSchemaScanIconData();
	
protected:
	bool operator== (const FGsSchemaScanIconData& __Other) const
	{
		return FGsSchemaScanIconData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaScanIconData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaScanIconData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaScanIconDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaScanIconDataRow() : Super()
	{
	}
	
	const FGsSchemaScanIconData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaScanIconData* Row = nullptr;
		return Table->FindRow<FGsSchemaScanIconData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaScanIconDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaScanIconDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableScanIconData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsScanIconType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaScanIconData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaScanIconData>(RowsMap))
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
	bool FindRowById(const EGsScanIconType& InId, OUT const FGsSchemaScanIconData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsScanIconType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaScanIconData* Temp;
		return !FindRowById(FGsSchemaScanIconData()._iconType, Temp);
	}
#endif
};
