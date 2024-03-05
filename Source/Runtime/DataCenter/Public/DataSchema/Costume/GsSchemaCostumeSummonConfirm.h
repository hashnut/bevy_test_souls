/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeSummonChangeCurrencyInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeSummonConfirm.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeSummonConfirm"))
struct DATACENTER_API FGsSchemaCostumeSummonConfirm
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
	TArray<FGsSchemaCostumeSummonChangeCurrencyInfo> currencyInfo;
public:
	FGsSchemaCostumeSummonConfirm();
	
protected:
	bool operator== (const FGsSchemaCostumeSummonConfirm& __Other) const
	{
		return FGsSchemaCostumeSummonConfirm::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeSummonConfirm::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeSummonConfirm::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeSummonConfirmRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeSummonConfirmRow() : Super()
	{
	}
	
	const FGsSchemaCostumeSummonConfirm* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeSummonConfirm* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeSummonConfirm>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeSummonConfirmRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeSummonConfirmRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeSummonConfirm final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Costume");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCostumeSummonConfirm*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCostumeSummonConfirm>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCostumeSummonConfirm*& OutRow) const
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
		const FGsSchemaCostumeSummonConfirm* Temp;
		return !FindRowById(FGsSchemaCostumeSummonConfirm().id, Temp);
	}
#endif
};
