/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardItemBagDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardItemBagDataExcel"))
struct DATACENTER_API FGsSchemaRewardItemBagDataExcel
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
	FGsSchemaRewardItemBagDataExcel();
	
protected:
	bool operator== (const FGsSchemaRewardItemBagDataExcel& __Other) const
	{
		return FGsSchemaRewardItemBagDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardItemBagDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardItemBagDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardItemBagDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardItemBagDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardItemBagDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardItemBagDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardItemBagDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardItemBagDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardItemBagDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardItemBagDataExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaRewardItemBagDataExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRewardItemBagDataExcel>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaRewardItemBagDataExcel*& OutRow) const
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
		const FGsSchemaRewardItemBagDataExcel* Temp;
		return !FindRowById(FGsSchemaRewardItemBagDataExcel().id, Temp);
	}
#endif
};
