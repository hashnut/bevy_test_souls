/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairySummonChangeCurrencyInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairySummonConfirm.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairySummonConfirm"))
struct DATACENTER_API FGsSchemaFairySummonConfirm
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="duration"))
	int32 duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyInfo"))
	TArray<FGsSchemaFairySummonChangeCurrencyInfo> currencyInfo;
public:
	FGsSchemaFairySummonConfirm();
	
protected:
	bool operator== (const FGsSchemaFairySummonConfirm& __Other) const
	{
		return FGsSchemaFairySummonConfirm::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairySummonConfirm::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairySummonConfirm::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairySummonConfirmRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairySummonConfirmRow() : Super()
	{
	}
	
	const FGsSchemaFairySummonConfirm* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairySummonConfirm* Row = nullptr;
		return Table->FindRow<FGsSchemaFairySummonConfirm>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairySummonConfirmRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairySummonConfirmRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairySummonConfirm final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFairySummonConfirm*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairySummonConfirm>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairySummonConfirm*& OutRow) const
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
		const FGsSchemaFairySummonConfirm* Temp;
		return !FindRowById(FGsSchemaFairySummonConfirm().id, Temp);
	}
#endif
};
