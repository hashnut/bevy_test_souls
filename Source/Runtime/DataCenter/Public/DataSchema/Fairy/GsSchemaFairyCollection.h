/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Fairy/GsSchemaFairyCollectionCondition.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyCollection.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyCollection"))
struct DATACENTER_API FGsSchemaFairyCollection
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="반드시 1개 이상"))
	FText name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyIdLevelList", Tooltip))
	TArray<FGsSchemaFairyCollectionCondition> fairyIdLevelList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityIdList", Tooltip))
	TArray<int32> passivityIdList;
	
public:
	FGsSchemaFairyCollection();
	
protected:
	bool operator== (const FGsSchemaFairyCollection& __Other) const
	{
		return FGsSchemaFairyCollection::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyCollection::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyCollection::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyCollectionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyCollectionRow() : Super()
	{
	}
	
	const FGsSchemaFairyCollection* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyCollection* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyCollection>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyCollectionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyCollectionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyCollection final : public UGsTable
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
		TMap<FName, const FGsSchemaFairyCollection*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairyCollection>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairyCollection*& OutRow) const
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
		const FGsSchemaFairyCollection* Temp;
		return !FindRowById(FGsSchemaFairyCollection().id, Temp);
	}
#endif
};
