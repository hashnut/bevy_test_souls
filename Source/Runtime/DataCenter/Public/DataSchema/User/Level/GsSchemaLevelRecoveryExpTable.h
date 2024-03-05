/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLevelRecoveryExpTable.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGsLevelRecoveryExpTable"))
struct DATACENTER_API FGsSchemaLevelRecoveryExpTable
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expRecoveryGoldCost"))
	int32 expRecoveryGoldCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expRecoveryDiaCost"))
	int32 expRecoveryDiaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expRecoveryItemCost"))
	int32 expRecoveryItemCost;
public:
	FGsSchemaLevelRecoveryExpTable();
	
protected:
	bool operator== (const FGsSchemaLevelRecoveryExpTable& __Other) const
	{
		return FGsSchemaLevelRecoveryExpTable::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLevelRecoveryExpTable::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLevelRecoveryExpTable::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLevelRecoveryExpTableRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLevelRecoveryExpTableRow() : Super()
	{
	}
	
	const FGsSchemaLevelRecoveryExpTable* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLevelRecoveryExpTable* Row = nullptr;
		return Table->FindRow<FGsSchemaLevelRecoveryExpTable>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLevelRecoveryExpTableRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLevelRecoveryExpTableRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLevelRecoveryExpTable final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User/Level");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaLevelRecoveryExpTable*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaLevelRecoveryExpTable>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->level, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaLevelRecoveryExpTable*& OutRow) const
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
		const FGsSchemaLevelRecoveryExpTable* Temp;
		return !FindRowById(FGsSchemaLevelRecoveryExpTable().level, Temp);
	}
#endif
};
