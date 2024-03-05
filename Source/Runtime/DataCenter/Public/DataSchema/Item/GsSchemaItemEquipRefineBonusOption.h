/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/GameObject/Stat/Data/GsStatTableStruct.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipRefineBonusOption.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipRefineBonusOption"))
struct DATACENTER_API FGsSchemaItemEquipRefineBonusOption
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineBonusOptionStatId"))
	int32 refineBonusOptionStatId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType"))
	FGsStatTypeNameSelector statType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayMinValue"))
	int32 displayMinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayMaxValue"))
	int32 displayMaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="valueList"))
	TArray<FGsItemRefineOptionPairData> valueList;
public:
	FGsSchemaItemEquipRefineBonusOption();
	
protected:
	bool operator== (const FGsSchemaItemEquipRefineBonusOption& __Other) const
	{
		return FGsSchemaItemEquipRefineBonusOption::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipRefineBonusOption::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipRefineBonusOption::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipRefineBonusOptionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipRefineBonusOptionRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipRefineBonusOption* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipRefineBonusOption* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipRefineBonusOption>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipRefineBonusOptionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipRefineBonusOptionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipRefineBonusOption final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEquipRefineBonusOption*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipRefineBonusOption>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->refineBonusOptionStatId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipRefineBonusOption*& OutRow) const
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
		const FGsSchemaItemEquipRefineBonusOption* Temp;
		return !FindRowById(FGsSchemaItemEquipRefineBonusOption().refineBonusOptionStatId, Temp);
	}
#endif
};
