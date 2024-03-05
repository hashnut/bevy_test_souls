/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/Data/GsItemStatData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipSetStatBalance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipSetStatBalance"))
struct DATACENTER_API FGsSchemaItemEquipSetStatBalance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="setId"))
	int32 setId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set2StatList"))
	TArray<FGsItemStatData> set2StatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set3StatList"))
	TArray<FGsItemStatData> set3StatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set4StatList"))
	TArray<FGsItemStatData> set4StatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set5StatList"))
	TArray<FGsItemStatData> set5StatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set6StatList"))
	TArray<FGsItemStatData> set6StatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set7StatList"))
	TArray<FGsItemStatData> set7StatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="set8StatList"))
	TArray<FGsItemStatData> set8StatList;
public:
	FGsSchemaItemEquipSetStatBalance();
	
protected:
	bool operator== (const FGsSchemaItemEquipSetStatBalance& __Other) const
	{
		return FGsSchemaItemEquipSetStatBalance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipSetStatBalance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipSetStatBalance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipSetStatBalanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipSetStatBalanceRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipSetStatBalance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipSetStatBalance* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipSetStatBalance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipSetStatBalanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipSetStatBalanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipSetStatBalance final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEquipSetStatBalance*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipSetStatBalance>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->setId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipSetStatBalance*& OutRow) const
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
		const FGsSchemaItemEquipSetStatBalance* Temp;
		return !FindRowById(FGsSchemaItemEquipSetStatBalance().setId, Temp);
	}
#endif
};
