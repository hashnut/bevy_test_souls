/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairySummon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectSummonFairy.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectSummonFairy"))
struct DATACENTER_API FGsSchemaItemEffectSummonFairy
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonCount"))
	int32 summonCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fairySummonId"))
	FGsSchemaFairySummonRow fairySummonId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="summonMileage"))
	int32 summonMileage;
public:
	FGsSchemaItemEffectSummonFairy();
	
protected:
	bool operator== (const FGsSchemaItemEffectSummonFairy& __Other) const
	{
		return FGsSchemaItemEffectSummonFairy::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectSummonFairy::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectSummonFairy::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectSummonFairyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectSummonFairyRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectSummonFairy* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectSummonFairy* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectSummonFairy>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectSummonFairyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectSummonFairyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectSummonFairy final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEffectSummonFairy*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectSummonFairy>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectSummonFairy*& OutRow) const
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
		const FGsSchemaItemEffectSummonFairy* Temp;
		return !FindRowById(FGsSchemaItemEffectSummonFairy().effectId, Temp);
	}
#endif
};
