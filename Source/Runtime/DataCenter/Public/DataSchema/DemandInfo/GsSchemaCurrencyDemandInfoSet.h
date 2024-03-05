/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCurrencyDemandInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="재화 사용처 세트", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/DemandInfo/BSGsCurrencyDemandInfoSet"))
struct DATACENTER_API FGsSchemaCurrencyDemandInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contentsTypeSet"))
	TSet<EGsUnlockContentsType> contentsTypeSet;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="craftIds"))
	TSet<int32> craftIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapIds"))
	TSet<int32> mapIds;
public:
	FGsSchemaCurrencyDemandInfoSet();
	
protected:
	bool operator== (const FGsSchemaCurrencyDemandInfoSet& __Other) const
	{
		return FGsSchemaCurrencyDemandInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCurrencyDemandInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCurrencyDemandInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCurrencyDemandInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCurrencyDemandInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaCurrencyDemandInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCurrencyDemandInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaCurrencyDemandInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCurrencyDemandInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCurrencyDemandInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCurrencyDemandInfoSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CurrencyType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("DemandInfo");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCurrencyDemandInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCurrencyDemandInfoSet>(RowsMap))
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
	bool FindRowById(const CurrencyType& InId, OUT const FGsSchemaCurrencyDemandInfoSet*& OutRow) const
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
		const FGsSchemaCurrencyDemandInfoSet* Temp;
		return !FindRowById(FGsSchemaCurrencyDemandInfoSet().currencyType, Temp);
	}
#endif
};
