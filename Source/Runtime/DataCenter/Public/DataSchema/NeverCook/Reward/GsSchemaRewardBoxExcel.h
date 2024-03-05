/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardBoxExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardBoxExcel"))
struct DATACENTER_API FGsSchemaRewardBoxExcel
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
	FGsSchemaRewardBoxExcel();
	
protected:
	bool operator== (const FGsSchemaRewardBoxExcel& __Other) const
	{
		return FGsSchemaRewardBoxExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardBoxExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardBoxExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardBoxExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardBoxExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardBoxExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardBoxExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardBoxExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardBoxExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardBoxExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardBoxExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaRewardBoxExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRewardBoxExcel>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaRewardBoxExcel*& OutRow) const
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
		const FGsSchemaRewardBoxExcel* Temp;
		return !FindRowById(FGsSchemaRewardBoxExcel().rewardBoxPairId, Temp);
	}
#endif
};
