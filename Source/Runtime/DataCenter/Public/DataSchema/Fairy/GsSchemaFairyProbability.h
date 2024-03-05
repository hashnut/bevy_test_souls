/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyProbabilityItem.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairySummonConfirm.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyProbability.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyProbability"))
struct DATACENTER_API FGsSchemaFairyProbability
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyList"))
	TArray<FGsSchemaFairyProbabilityItem> fairyList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="summonConfirmId"))
	FGsSchemaFairySummonConfirmRow summonConfirmId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="자신의 테이블 id 임", DisplayName="nextProbabilityId"))
	int32 nextProbabilityId;
public:
	FGsSchemaFairyProbability();
	
protected:
	bool operator== (const FGsSchemaFairyProbability& __Other) const
	{
		return FGsSchemaFairyProbability::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyProbability::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyProbability::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyProbabilityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyProbabilityRow() : Super()
	{
	}
	
	const FGsSchemaFairyProbability* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyProbability* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyProbability>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyProbabilityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyProbabilityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyProbability final : public UGsTable
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
		TMap<FName, const FGsSchemaFairyProbability*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairyProbability>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairyProbability*& OutRow) const
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
		const FGsSchemaFairyProbability* Temp;
		return !FindRowById(FGsSchemaFairyProbability().id, Temp);
	}
#endif
};
