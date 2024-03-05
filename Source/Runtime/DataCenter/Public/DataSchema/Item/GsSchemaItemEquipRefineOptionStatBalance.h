/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Item/GsSchemaRefineOptionStatBalanceData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaItemEquipRefineOptionStatBalance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipRefineOptionStatBalance"))
struct DATACENTER_API FGsSchemaItemEquipRefineOptionStatBalance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineOptionStatId"))
	int32 refineOptionStatId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat"))
	FGsStatTypeNameSelector stat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="displayMinValue", Tooltip="클라에서 출력될 최소 스텟값"))
	int32 displayMinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="displayMaxValue", Tooltip="클라에서 출력될 최대 스텟값"))
	int32 displayMaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValueList"))
	TArray<FGsSchemaRefineOptionStatBalanceData> statValueList;
public:
	FGsSchemaItemEquipRefineOptionStatBalance();
	
protected:
	bool operator== (const FGsSchemaItemEquipRefineOptionStatBalance& __Other) const
	{
		return FGsSchemaItemEquipRefineOptionStatBalance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipRefineOptionStatBalance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipRefineOptionStatBalance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipRefineOptionStatBalanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipRefineOptionStatBalanceRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipRefineOptionStatBalance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipRefineOptionStatBalance* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipRefineOptionStatBalance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipRefineOptionStatBalanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipRefineOptionStatBalanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipRefineOptionStatBalance final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEquipRefineOptionStatBalance*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipRefineOptionStatBalance>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->refineOptionStatId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipRefineOptionStatBalance*& OutRow) const
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
		const FGsSchemaItemEquipRefineOptionStatBalance* Temp;
		return !FindRowById(FGsSchemaItemEquipRefineOptionStatBalance().refineOptionStatId, Temp);
	}
#endif
};
