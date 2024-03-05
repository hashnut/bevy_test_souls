/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfoCreatureIds.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemSupplyInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="아이템 공급처 정보 세트", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SupplyInfo/BSGsItemSupplyInfoSet"))
struct DATACENTER_API FGsSchemaItemSupplyInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contentsTypeSet"))
	TSet<EGsUnlockContentsType> contentsTypeSet;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="contentsEventMapCreatureSetMap"))
	TMap<int32,FGsSchemaDropSupplyInfo> contentsEventMapCreatureSetMap;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="mapCreatureSetMap"))
	TMap<int32,FGsSchemaDropSupplyInfoCreatureIds> mapCreatureSetMap;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="craftIdSet"))
	TSet<int32> craftIdSet;
public:
	FGsSchemaItemSupplyInfoSet();
	
protected:
	bool operator== (const FGsSchemaItemSupplyInfoSet& __Other) const
	{
		return FGsSchemaItemSupplyInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemSupplyInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemSupplyInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemSupplyInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemSupplyInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaItemSupplyInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemSupplyInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaItemSupplyInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemSupplyInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemSupplyInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemSupplyInfoSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SupplyInfo");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemSupplyInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemSupplyInfoSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemSupplyInfoSet*& OutRow) const
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
		const FGsSchemaItemSupplyInfoSet* Temp;
		return !FindRowById(FGsSchemaItemSupplyInfoSet().itemId, Temp);
	}
#endif
};
