/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropExpGoldGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Drop/BSGsDropExpGoldGroup"))
struct DATACENTER_API FGsSchemaDropExpGoldGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gold"))
	int32 gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exp"))
	int32 exp;
public:
	FGsSchemaDropExpGoldGroup();
	
protected:
	bool operator== (const FGsSchemaDropExpGoldGroup& __Other) const
	{
		return FGsSchemaDropExpGoldGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDropExpGoldGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDropExpGoldGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDropExpGoldGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDropExpGoldGroupRow() : Super()
	{
	}
	
	const FGsSchemaDropExpGoldGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDropExpGoldGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaDropExpGoldGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDropExpGoldGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDropExpGoldGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDropExpGoldGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Drop");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaDropExpGoldGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaDropExpGoldGroup>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaDropExpGoldGroup*& OutRow) const
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
		const FGsSchemaDropExpGoldGroup* Temp;
		return !FindRowById(FGsSchemaDropExpGoldGroup().level, Temp);
	}
#endif
};
