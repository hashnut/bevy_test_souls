/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeSummon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectSummonCostume.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectSummonCostume"))
struct DATACENTER_API FGsSchemaItemEffectSummonCostume
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonCount"))
	int32 summonCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="costumeSummonId"))
	FGsSchemaCostumeSummonRow costumeSummonId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonMileage"))
	int32 summonMileage;
public:
	FGsSchemaItemEffectSummonCostume();
	
protected:
	bool operator== (const FGsSchemaItemEffectSummonCostume& __Other) const
	{
		return FGsSchemaItemEffectSummonCostume::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectSummonCostume::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectSummonCostume::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectSummonCostumeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectSummonCostumeRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectSummonCostume* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectSummonCostume* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectSummonCostume>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectSummonCostumeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectSummonCostumeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectSummonCostume final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectSummonCostume*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectSummonCostume>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectSummonCostume*& OutRow) const
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
		const FGsSchemaItemEffectSummonCostume* Temp;
		return !FindRowById(FGsSchemaItemEffectSummonCostume().effectId, Temp);
	}
#endif
};
