/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestMainChapter.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Main/BSGsQuestMainChapter"))
struct DATACENTER_API FGsSchemaQuestMainChapter
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exposeQuest"))
	bool exposeQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="chapterTitleText"))
	FText chapterTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questStoryList"))
	TArray<FGsSchemaQuestMainStoryRow> questStoryList;
public:
	FGsSchemaQuestMainChapter();
	
protected:
	bool operator== (const FGsSchemaQuestMainChapter& __Other) const
	{
		return FGsSchemaQuestMainChapter::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestMainChapter::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestMainChapter::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestMainChapterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestMainChapterRow() : Super()
	{
	}
	
	const FGsSchemaQuestMainChapter* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestMainChapter* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestMainChapter>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestMainChapterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestMainChapterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestMainChapter final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Main");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestMainChapter*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestMainChapter>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestMainChapter*& OutRow) const
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
		const FGsSchemaQuestMainChapter* Temp;
		return !FindRowById(FGsSchemaQuestMainChapter().id, Temp);
	}
#endif
};
