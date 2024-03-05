/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemDemandInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="아이템 사용처 세트", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/DemandInfo/BSGsItemDemandInfoSet"))
struct DATACENTER_API FGsSchemaItemDemandInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contentsTypeSet"))
	TSet<EGsUnlockContentsType> contentsTypeSet;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="craftIds"))
	TSet<int32> craftIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapIds"))
	TSet<int32> mapIds;
public:
	FGsSchemaItemDemandInfoSet();
	
protected:
	bool operator== (const FGsSchemaItemDemandInfoSet& __Other) const
	{
		return FGsSchemaItemDemandInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemDemandInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemDemandInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemDemandInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemDemandInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaItemDemandInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemDemandInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaItemDemandInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemDemandInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemDemandInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemDemandInfoSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("DemandInfo");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemDemandInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemDemandInfoSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemDemandInfoSet*& OutRow) const
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
		const FGsSchemaItemDemandInfoSet* Temp;
		return !FindRowById(FGsSchemaItemDemandInfoSet().itemId, Temp);
	}
#endif
};
