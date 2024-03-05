/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/QuestBoard/GsSchemaQuestConditionInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaVillageQuestBoardList.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/QuestBoard/BSGsVillageQuestBoardList"))
struct DATACENTER_API FGsSchemaVillageQuestBoardList
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestConditionInfo"))
	TArray<FGsSchemaQuestConditionInfo> QuestConditionInfo;
public:
	FGsSchemaVillageQuestBoardList();
	
protected:
	bool operator== (const FGsSchemaVillageQuestBoardList& __Other) const
	{
		return FGsSchemaVillageQuestBoardList::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaVillageQuestBoardList::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaVillageQuestBoardList::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaVillageQuestBoardListRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaVillageQuestBoardListRow() : Super()
	{
	}
	
	const FGsSchemaVillageQuestBoardList* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaVillageQuestBoardList* Row = nullptr;
		return Table->FindRow<FGsSchemaVillageQuestBoardList>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaVillageQuestBoardListRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaVillageQuestBoardListRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableVillageQuestBoardList final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("QuestBoard");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaVillageQuestBoardList*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaVillageQuestBoardList>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaVillageQuestBoardList*& OutRow) const
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
		const FGsSchemaVillageQuestBoardList* Temp;
		return !FindRowById(FGsSchemaVillageQuestBoardList().id, Temp);
	}
#endif
};
