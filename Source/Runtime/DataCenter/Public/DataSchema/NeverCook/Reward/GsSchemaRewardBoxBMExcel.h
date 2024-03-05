/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardBoxBMExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardBoxBMExcel"))
struct DATACENTER_API FGsSchemaRewardBoxBMExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardBoxPairId"))
	int64 rewardBoxPairId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardBoxId"))
	int64 rewardBoxId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rate"))
	int32 rate;
public:
	FGsSchemaRewardBoxBMExcel();
	
protected:
	bool operator== (const FGsSchemaRewardBoxBMExcel& __Other) const
	{
		return FGsSchemaRewardBoxBMExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardBoxBMExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardBoxBMExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardBoxBMExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardBoxBMExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardBoxBMExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardBoxBMExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardBoxBMExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardBoxBMExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardBoxBMExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardBoxBMExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaRewardBoxBMExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRewardBoxBMExcel>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->rewardBoxPairId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaRewardBoxBMExcel*& OutRow) const
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
		const FGsSchemaRewardBoxBMExcel* Temp;
		return !FindRowById(FGsSchemaRewardBoxBMExcel().rewardBoxPairId, Temp);
	}
#endif
};
