/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaObjectiveWarpToLastPublicFieldSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsObjectiveWarpToLastPublicFieldSet"))
struct DATACENTER_API FGsSchemaObjectiveWarpToLastPublicFieldSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Id"))
	int64 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pos"))
	FGsSchemaQuestObjectiveAreaPos pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="delayTime"))
	float delayTime;
public:
	FGsSchemaObjectiveWarpToLastPublicFieldSet();
	
protected:
	bool operator== (const FGsSchemaObjectiveWarpToLastPublicFieldSet& __Other) const
	{
		return FGsSchemaObjectiveWarpToLastPublicFieldSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaObjectiveWarpToLastPublicFieldSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaObjectiveWarpToLastPublicFieldSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaObjectiveWarpToLastPublicFieldSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaObjectiveWarpToLastPublicFieldSetRow() : Super()
	{
	}
	
	const FGsSchemaObjectiveWarpToLastPublicFieldSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaObjectiveWarpToLastPublicFieldSet* Row = nullptr;
		return Table->FindRow<FGsSchemaObjectiveWarpToLastPublicFieldSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaObjectiveWarpToLastPublicFieldSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaObjectiveWarpToLastPublicFieldSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableObjectiveWarpToLastPublicFieldSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaObjectiveWarpToLastPublicFieldSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaObjectiveWarpToLastPublicFieldSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->Id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaObjectiveWarpToLastPublicFieldSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaObjectiveWarpToLastPublicFieldSet* Temp;
		return !FindRowById(FGsSchemaObjectiveWarpToLastPublicFieldSet().Id, Temp);
	}
#endif
};
