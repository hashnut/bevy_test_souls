/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardBoxEtcExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardBoxEtcExcel"))
struct DATACENTER_API FGsSchemaRewardBoxEtcExcel
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
	FGsSchemaRewardBoxEtcExcel();
	
protected:
	bool operator== (const FGsSchemaRewardBoxEtcExcel& __Other) const
	{
		return FGsSchemaRewardBoxEtcExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardBoxEtcExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardBoxEtcExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardBoxEtcExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardBoxEtcExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardBoxEtcExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardBoxEtcExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardBoxEtcExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardBoxEtcExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardBoxEtcExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardBoxEtcExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaRewardBoxEtcExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRewardBoxEtcExcel>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaRewardBoxEtcExcel*& OutRow) const
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
		const FGsSchemaRewardBoxEtcExcel* Temp;
		return !FindRowById(FGsSchemaRewardBoxEtcExcel().rewardBoxPairId, Temp);
	}
#endif
};
