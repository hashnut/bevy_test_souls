/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsImmuneStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Item/GsResistStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityCategorySet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/Category/BSGsAbnormalityCategorySet"))
struct DATACENTER_API FGsSchemaAbnormalityCategorySet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine="true", DisplayName="categoryDesc"))
	FString categoryDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="immuneDealScrollTextMe"))
	FName immuneDealScrollTextMe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="immuneDealScrollTextTarget"))
	FName immuneDealScrollTextTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="resistStatName"))
	FString resistStatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resistStatType"))
	FGsResistStatTypeNameSelector resistStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="immuneStatType"))
	FGsImmuneStatTypeNameSelector immuneStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hitStatType"))
	FGsStatTypeNameSelector hitStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="timeReductionStatType"))
	FGsResistStatTypeNameSelector timeReductionStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="timeIncreaseStatType"))
	FGsResistStatTypeNameSelector timeIncreaseStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Control_FactorPc"))
	int32 Control_FactorPc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Control_FactorNpc"))
	int32 Control_FactorNpc;
public:
	FGsSchemaAbnormalityCategorySet();
	
protected:
	bool operator== (const FGsSchemaAbnormalityCategorySet& __Other) const
	{
		return FGsSchemaAbnormalityCategorySet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityCategorySet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityCategorySet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityCategorySetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityCategorySetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityCategorySet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityCategorySet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityCategorySet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityCategorySetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityCategorySetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityCategorySet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality/Category");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAbnormalityCategorySet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAbnormalityCategorySet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaAbnormalityCategorySet*& OutRow) const
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
		const FGsSchemaAbnormalityCategorySet* Temp;
		return !FindRowById(FGsSchemaAbnormalityCategorySet().id, Temp);
	}
#endif
};
