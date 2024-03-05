/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeProbabilityItem.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeSummonConfirm.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeProbability.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeProbability"))
struct DATACENTER_API FGsSchemaCostumeProbability
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeList"))
	TArray<FGsSchemaCostumeProbabilityItem> costumeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="summonConfirmId"))
	FGsSchemaCostumeSummonConfirmRow summonConfirmId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="자신의 테이블 id 임", DisplayName="nextProbabilityId"))
	int32 nextProbabilityId;
public:
	FGsSchemaCostumeProbability();
	
protected:
	bool operator== (const FGsSchemaCostumeProbability& __Other) const
	{
		return FGsSchemaCostumeProbability::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeProbability::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeProbability::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeProbabilityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeProbabilityRow() : Super()
	{
	}
	
	const FGsSchemaCostumeProbability* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeProbability* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeProbability>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeProbabilityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeProbabilityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeProbability final : public UGsTable
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
		TMap<FName, const FGsSchemaCostumeProbability*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCostumeProbability>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCostumeProbability*& OutRow) const
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
		const FGsSchemaCostumeProbability* Temp;
		return !FindRowById(FGsSchemaCostumeProbability().id, Temp);
	}
#endif
};
