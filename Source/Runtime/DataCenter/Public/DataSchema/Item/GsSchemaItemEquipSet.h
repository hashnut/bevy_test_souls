/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipSet"))
struct DATACENTER_API FGsSchemaItemEquipSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="setId"))
	int32 setId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId1"))
	int32 itemId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId2"))
	int32 itemId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId3"))
	int32 itemId3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId4"))
	int32 itemId4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId5"))
	int32 itemId5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId6"))
	int32 itemId6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId7"))
	int32 itemId7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId8"))
	int32 itemId8;
public:
	FGsSchemaItemEquipSet();
	
protected:
	bool operator== (const FGsSchemaItemEquipSet& __Other) const
	{
		return FGsSchemaItemEquipSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipSetRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipSet* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipSet final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEquipSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->setId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipSet*& OutRow) const
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
		const FGsSchemaItemEquipSet* Temp;
		return !FindRowById(FGsSchemaItemEquipSet().setId, Temp);
	}
#endif
};
