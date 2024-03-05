/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardItemBagDataItemExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardItemBagDataItemExcel"))
struct DATACENTER_API FGsSchemaRewardItemBagDataItemExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmountMin"))
	int32 currencyAmountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmountMax"))
	int32 currencyAmountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCountMin"))
	int32 itemCountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCountMax"))
	int32 itemCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemLevel"))
	int32 itemLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantShield"))
	int32 enchantShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantBonusIndex"))
	int32 enchantBonusIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int32 probability;
public:
	FGsSchemaRewardItemBagDataItemExcel();
	
protected:
	bool operator== (const FGsSchemaRewardItemBagDataItemExcel& __Other) const
	{
		return FGsSchemaRewardItemBagDataItemExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardItemBagDataItemExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardItemBagDataItemExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardItemBagDataItemExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardItemBagDataItemExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardItemBagDataItemExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardItemBagDataItemExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardItemBagDataItemExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardItemBagDataItemExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardItemBagDataItemExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardItemBagDataItemExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Reward");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaRewardItemBagDataItemExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRewardItemBagDataItemExcel>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaRewardItemBagDataItemExcel*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaRewardItemBagDataItemExcel* Temp;
		return !FindRowById(FGsSchemaRewardItemBagDataItemExcel().id, Temp);
	}
#endif
};
