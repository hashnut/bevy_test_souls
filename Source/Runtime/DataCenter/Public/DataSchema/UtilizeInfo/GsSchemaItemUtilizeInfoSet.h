/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/UtilizeInfo/GsSchemaUtilizeMapInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemUtilizeInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UtilizeInfo/BSGsItemUtilizeInfoSet"))
struct DATACENTER_API FGsSchemaItemUtilizeInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="utilizeMapInfo"))
	TArray<FGsSchemaUtilizeMapInfo> utilizeMapInfo;
public:
	FGsSchemaItemUtilizeInfoSet();
	
protected:
	bool operator== (const FGsSchemaItemUtilizeInfoSet& __Other) const
	{
		return FGsSchemaItemUtilizeInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemUtilizeInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemUtilizeInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemUtilizeInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemUtilizeInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaItemUtilizeInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemUtilizeInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaItemUtilizeInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemUtilizeInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemUtilizeInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemUtilizeInfoSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("UtilizeInfo");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemUtilizeInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemUtilizeInfoSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemUtilizeInfoSet*& OutRow) const
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
		const FGsSchemaItemUtilizeInfoSet* Temp;
		return !FindRowById(FGsSchemaItemUtilizeInfoSet().itemId, Temp);
	}
#endif
};
