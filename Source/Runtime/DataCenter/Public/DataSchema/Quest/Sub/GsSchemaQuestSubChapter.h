/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Icon/GsSchemaQuestImg.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Sub/GsSchemaQuestSubStory.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestSubChapter.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="퀘스트 서브 챕터데이터", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Sub/BSGsQuestSubChapter"))
struct DATACENTER_API FGsSchemaQuestSubChapter
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exposeQuest"))
	bool exposeQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcId"))
	FGsSchemaNpcDataRow npcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcIcon"))
	FGsSchemaQuestImgRow npcIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcPos"))
	FGsSchemaQuestObjectiveAreaPos npcPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questStoryList"))
	TArray<FGsSchemaQuestSubStoryRow> questStoryList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questContentsType"))
	QuestContentsType questContentsType;
public:
	FGsSchemaQuestSubChapter();
	
protected:
	bool operator== (const FGsSchemaQuestSubChapter& __Other) const
	{
		return FGsSchemaQuestSubChapter::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestSubChapter::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestSubChapter::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestSubChapterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestSubChapterRow() : Super()
	{
	}
	
	const FGsSchemaQuestSubChapter* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestSubChapter* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestSubChapter>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestSubChapterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestSubChapterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestSubChapter final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Sub");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestSubChapter*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestSubChapter>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestSubChapter*& OutRow) const
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
		const FGsSchemaQuestSubChapter* Temp;
		return !FindRowById(FGsSchemaQuestSubChapter().id, Temp);
	}
#endif
};
