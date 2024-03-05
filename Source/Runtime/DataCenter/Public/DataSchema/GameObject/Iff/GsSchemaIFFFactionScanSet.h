/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIFFFactionScanSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Iff/BSGsIFFFactionScanSet"))
struct DATACENTER_API FGsSchemaIFFFactionScanSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iffFactionType"))
	IffFactionType iffFactionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemyFactionTypeList"))
	TArray<IffFactionType> enemyFactionTypeList;
public:
	FGsSchemaIFFFactionScanSet();
	
protected:
	bool operator== (const FGsSchemaIFFFactionScanSet& __Other) const
	{
		return FGsSchemaIFFFactionScanSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaIFFFactionScanSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaIFFFactionScanSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaIFFFactionScanSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaIFFFactionScanSetRow() : Super()
	{
	}
	
	const FGsSchemaIFFFactionScanSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaIFFFactionScanSet* Row = nullptr;
		return Table->FindRow<FGsSchemaIFFFactionScanSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaIFFFactionScanSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaIFFFactionScanSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableIFFFactionScanSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<IffFactionType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Iff");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaIFFFactionScanSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaIFFFactionScanSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->iffFactionType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const IffFactionType& InId, OUT const FGsSchemaIFFFactionScanSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const IffFactionType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaIFFFactionScanSet* Temp;
		return !FindRowById(FGsSchemaIFFFactionScanSet().iffFactionType, Temp);
	}
#endif
};
