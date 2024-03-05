/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestMultiLevelRankStory.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="퀘스트 랭크 스토리데이터", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Rank/BSGsQuestMultiLevelRankStory"))
struct DATACENTER_API FGsSchemaQuestMultiLevelRankStory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questIdList"))
	TArray<FGsSchemaQuestRow> questIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconList"))
	TArray<FSoftObjectPath> iconList;
public:
	FGsSchemaQuestMultiLevelRankStory();
	
protected:
	bool operator== (const FGsSchemaQuestMultiLevelRankStory& __Other) const
	{
		return FGsSchemaQuestMultiLevelRankStory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestMultiLevelRankStory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestMultiLevelRankStory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestMultiLevelRankStoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestMultiLevelRankStoryRow() : Super()
	{
	}
	
	const FGsSchemaQuestMultiLevelRankStory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestMultiLevelRankStory* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestMultiLevelRankStory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestMultiLevelRankStoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestMultiLevelRankStoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestMultiLevelRankStory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Rank");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestMultiLevelRankStory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestMultiLevelRankStory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestMultiLevelRankStory*& OutRow) const
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
		const FGsSchemaQuestMultiLevelRankStory* Temp;
		return !FindRowById(FGsSchemaQuestMultiLevelRankStory().id, Temp);
	}
#endif
};
