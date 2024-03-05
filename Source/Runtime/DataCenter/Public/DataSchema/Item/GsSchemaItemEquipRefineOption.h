/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Item/GsSchemaRefineOptionData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipRefineOption.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipRefineOption"))
struct DATACENTER_API FGsSchemaItemEquipRefineOption
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineOptionId"))
	int32 refineOptionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineOptionStatList"))
	TArray<FGsSchemaRefineOptionData> refineOptionStatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="blessedRefineOptionStatList"))
	TArray<FGsSchemaRefineOptionData> blessedRefineOptionStatList;
public:
	FGsSchemaItemEquipRefineOption();
	
protected:
	bool operator== (const FGsSchemaItemEquipRefineOption& __Other) const
	{
		return FGsSchemaItemEquipRefineOption::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipRefineOption::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipRefineOption::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipRefineOptionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipRefineOptionRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipRefineOption* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipRefineOption* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipRefineOption>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipRefineOptionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipRefineOptionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipRefineOption final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEquipRefineOption*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipRefineOption>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->refineOptionId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipRefineOption*& OutRow) const
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
		const FGsSchemaItemEquipRefineOption* Temp;
		return !FindRowById(FGsSchemaItemEquipRefineOption().refineOptionId, Temp);
	}
#endif
};
