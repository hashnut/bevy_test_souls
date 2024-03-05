/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Costume/GsSchemaCostumeCollectionCondition.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeCollection.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeCollection"))
struct DATACENTER_API FGsSchemaCostumeCollection
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="반드시 1개 이상"))
	FText name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeIdList", Tooltip))
	TArray<FGsSchemaCostumeCollectionCondition> costumeIdList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityIdList", Tooltip))
	TArray<int32> passivityIdList;
	
public:
	FGsSchemaCostumeCollection();
	
protected:
	bool operator== (const FGsSchemaCostumeCollection& __Other) const
	{
		return FGsSchemaCostumeCollection::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeCollection::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeCollection::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeCollectionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeCollectionRow() : Super()
	{
	}
	
	const FGsSchemaCostumeCollection* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeCollection* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeCollection>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeCollectionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeCollectionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeCollection final : public UGsTable
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
		TMap<FName, const FGsSchemaCostumeCollection*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCostumeCollection>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCostumeCollection*& OutRow) const
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
		const FGsSchemaCostumeCollection* Temp;
		return !FindRowById(FGsSchemaCostumeCollection().id, Temp);
	}
#endif
};
