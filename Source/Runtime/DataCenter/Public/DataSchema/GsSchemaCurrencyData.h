/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/DemandInfo/GsSchemaDemandInfoConstraint.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaSupplyInfoConstraint.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCurrencyData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsCurrencyData"))
struct DATACENTER_API FGsSchemaCurrencyData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MultiLine="true", DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MultiLine="true", DisplayName="acq"))
	FText acq;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MultiLine="true", DisplayName="usedfor"))
	FText usedfor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropDownOrder"))
	int32 dropDownOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="supplyInfoConstraint"))
	FGsSchemaSupplyInfoConstraintRow supplyInfoConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="demandInfoConstraint"))
	FGsSchemaDemandInfoConstraintRow demandInfoConstraint;
public:
	FGsSchemaCurrencyData();
	
protected:
	bool operator== (const FGsSchemaCurrencyData& __Other) const
	{
		return FGsSchemaCurrencyData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCurrencyData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCurrencyData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCurrencyDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCurrencyDataRow() : Super()
	{
	}
	
	const FGsSchemaCurrencyData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCurrencyData* Row = nullptr;
		return Table->FindRow<FGsSchemaCurrencyData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCurrencyDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCurrencyDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCurrencyData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CurrencyType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCurrencyData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCurrencyData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->currencyType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CurrencyType& InId, OUT const FGsSchemaCurrencyData*& OutRow) const
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
		const FGsSchemaCurrencyData* Temp;
		return !FindRowById(FGsSchemaCurrencyData().currencyType, Temp);
	}
#endif
};
