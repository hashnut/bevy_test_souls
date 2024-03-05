/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfoCreatureIds.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCurrencySupplyInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="재화 공급처 정보 세트", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SupplyInfo/BSGsCurrencySupplyInfoSet"))
struct DATACENTER_API FGsSchemaCurrencySupplyInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contentsTypeSet"))
	TSet<EGsUnlockContentsType> contentsTypeSet;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="contentsEventMapCreatureSetMap"))
	TMap<int32,FGsSchemaDropSupplyInfo> contentsEventMapCreatureSetMap;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="mapCreatureSetMap"))
	TMap<int32,FGsSchemaDropSupplyInfoCreatureIds> mapCreatureSetMap;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="craftIdSet"))
	TSet<int32> craftIdSet;
public:
	FGsSchemaCurrencySupplyInfoSet();
	
protected:
	bool operator== (const FGsSchemaCurrencySupplyInfoSet& __Other) const
	{
		return FGsSchemaCurrencySupplyInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCurrencySupplyInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCurrencySupplyInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCurrencySupplyInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCurrencySupplyInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaCurrencySupplyInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCurrencySupplyInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaCurrencySupplyInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCurrencySupplyInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCurrencySupplyInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCurrencySupplyInfoSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CurrencyType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SupplyInfo");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCurrencySupplyInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCurrencySupplyInfoSet>(RowsMap))
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
	bool FindRowById(const CurrencyType& InId, OUT const FGsSchemaCurrencySupplyInfoSet*& OutRow) const
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
		const FGsSchemaCurrencySupplyInfoSet* Temp;
		return !FindRowById(FGsSchemaCurrencySupplyInfoSet().currencyType, Temp);
	}
#endif
};
