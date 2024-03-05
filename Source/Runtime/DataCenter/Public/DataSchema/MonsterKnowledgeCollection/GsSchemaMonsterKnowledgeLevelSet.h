/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterKnowledgeLevelSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/BSGsMonsterKnowledgeLevelSet"))
struct DATACENTER_API FGsSchemaMonsterKnowledgeLevelSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="knowledgeGainProbability"))
	int32 knowledgeGainProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="knowledgeGainValue"))
	int32 knowledgeGainValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="knowledgeSurplusProbability"))
	int32 knowledgeSurplusProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="knowledgeSurplusItem"))
	FGsSchemaItemCommonRow knowledgeSurplusItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="knowledgeSurplusItemAmount"))
	int32 knowledgeSurplusItemAmount;
public:
	FGsSchemaMonsterKnowledgeLevelSet();
	
protected:
	bool operator== (const FGsSchemaMonsterKnowledgeLevelSet& __Other) const
	{
		return FGsSchemaMonsterKnowledgeLevelSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterKnowledgeLevelSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterKnowledgeLevelSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterKnowledgeLevelSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterKnowledgeLevelSetRow() : Super()
	{
	}
	
	const FGsSchemaMonsterKnowledgeLevelSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterKnowledgeLevelSet* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterKnowledgeLevelSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterKnowledgeLevelSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterKnowledgeLevelSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterKnowledgeLevelSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MonsterKnowledgeCollection");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMonsterKnowledgeLevelSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMonsterKnowledgeLevelSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMonsterKnowledgeLevelSet*& OutRow) const
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
		const FGsSchemaMonsterKnowledgeLevelSet* Temp;
		return !FindRowById(FGsSchemaMonsterKnowledgeLevelSet().id, Temp);
	}
#endif
};
