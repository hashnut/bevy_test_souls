/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsSchemaTitleConditionDataCurrency.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataCurrency"))
struct DATACENTER_API FGsSchemaTitleConditionDataCurrency
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCurrencyType"))
	CurrencyType mCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCurrency"))
	int64 mCurrency;
public:
	FGsSchemaTitleConditionDataCurrency();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataCurrency& __Other) const
	{
		return FGsSchemaTitleConditionDataCurrency::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataCurrency::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataCurrency::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataCurrencyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataCurrencyRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataCurrency* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataCurrency* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataCurrency>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataCurrencyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataCurrencyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataCurrency final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CurrencyType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitleConditionDataCurrency*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitleConditionDataCurrency>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mCurrencyType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CurrencyType& InId, OUT const FGsSchemaTitleConditionDataCurrency*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CurrencyType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaTitleConditionDataCurrency* Temp;
		return !FindRowById(FGsSchemaTitleConditionDataCurrency().mCurrencyType, Temp);
	}
#endif
};
