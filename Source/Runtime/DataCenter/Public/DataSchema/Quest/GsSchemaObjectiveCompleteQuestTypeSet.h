/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaObjectiveCompleteQuestTypeSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsObjectiveCompleteQuestTypeSet"))
struct DATACENTER_API FGsSchemaObjectiveCompleteQuestTypeSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questType"))
	QuestType questType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="count"))
	int32 count;
public:
	FGsSchemaObjectiveCompleteQuestTypeSet();
	
protected:
	bool operator== (const FGsSchemaObjectiveCompleteQuestTypeSet& __Other) const
	{
		return FGsSchemaObjectiveCompleteQuestTypeSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaObjectiveCompleteQuestTypeSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaObjectiveCompleteQuestTypeSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaObjectiveCompleteQuestTypeSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaObjectiveCompleteQuestTypeSetRow() : Super()
	{
	}
	
	const FGsSchemaObjectiveCompleteQuestTypeSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaObjectiveCompleteQuestTypeSet* Row = nullptr;
		return Table->FindRow<FGsSchemaObjectiveCompleteQuestTypeSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaObjectiveCompleteQuestTypeSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaObjectiveCompleteQuestTypeSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableObjectiveCompleteQuestTypeSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaObjectiveCompleteQuestTypeSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaObjectiveCompleteQuestTypeSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaObjectiveCompleteQuestTypeSet*& OutRow) const
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
		const FGsSchemaObjectiveCompleteQuestTypeSet* Temp;
		return !FindRowById(FGsSchemaObjectiveCompleteQuestTypeSet().id, Temp);
	}
#endif
};
