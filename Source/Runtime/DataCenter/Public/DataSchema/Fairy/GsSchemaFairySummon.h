/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairySummonItem.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairySummon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairySummon"))
struct DATACENTER_API FGsSchemaFairySummon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probabilityList"))
	TArray<FGsSchemaFairySummonItem> probabilityList;
public:
	FGsSchemaFairySummon();
	
protected:
	bool operator== (const FGsSchemaFairySummon& __Other) const
	{
		return FGsSchemaFairySummon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairySummon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairySummon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairySummonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairySummonRow() : Super()
	{
	}
	
	const FGsSchemaFairySummon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairySummon* Row = nullptr;
		return Table->FindRow<FGsSchemaFairySummon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairySummonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairySummonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairySummon final : public UGsTable
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
		TMap<FName, const FGsSchemaFairySummon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairySummon>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairySummon*& OutRow) const
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
		const FGsSchemaFairySummon* Temp;
		return !FindRowById(FGsSchemaFairySummon().id, Temp);
	}
#endif
};
