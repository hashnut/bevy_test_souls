/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealSlot.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인슬롯", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealSlot"))
struct DATACENTER_API FGsSchemaSealSlot
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인 노드 ID", DisplayName="sealNodeId"))
	int32 sealNodeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리롤 획득 ID", DisplayName="rerollGainId"))
	int32 rerollGainId;
public:
	FGsSchemaSealSlot();
	
protected:
	bool operator== (const FGsSchemaSealSlot& __Other) const
	{
		return FGsSchemaSealSlot::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealSlot::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealSlot::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealSlotRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealSlotRow() : Super()
	{
	}
	
	const FGsSchemaSealSlot* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealSlot* Row = nullptr;
		return Table->FindRow<FGsSchemaSealSlot>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealSlotRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealSlotRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealSlot final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSealSlot*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSealSlot>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSealSlot*& OutRow) const
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
		const FGsSchemaSealSlot* Temp;
		return !FindRowById(FGsSchemaSealSlot().id, Temp);
	}
#endif
};
