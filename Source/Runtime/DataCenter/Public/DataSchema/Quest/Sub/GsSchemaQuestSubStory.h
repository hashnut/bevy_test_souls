/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogData.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestSubStory.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="퀘스트 서브 스토리데이터", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Sub/BSGsQuestSubStory"))
struct DATACENTER_API FGsSchemaQuestSubStory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exposeQuest"))
	bool exposeQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireLevel"))
	int32 requireLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyTitleText"))
	FText storyTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyDescriptionText"))
	FText storyDescriptionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questIdList"))
	TArray<FGsSchemaQuestRow> questIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mainRewardId"))
	FGsSchemaRewardDataRow mainRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subRewardId"))
	FGsSchemaRewardDataRow subRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questBeginNpcDialogId"))
	FGsSchemaNpcDialogDataRow questBeginNpcDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireMainStoryId"))
	int32 requireMainStoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireSubStoryId"))
	int32 requireSubStoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isGoToNpcTeleportAble"))
	bool isGoToNpcTeleportAble;
public:
	FGsSchemaQuestSubStory();
	
protected:
	bool operator== (const FGsSchemaQuestSubStory& __Other) const
	{
		return FGsSchemaQuestSubStory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestSubStory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestSubStory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestSubStoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestSubStoryRow() : Super()
	{
	}
	
	const FGsSchemaQuestSubStory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestSubStory* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestSubStory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestSubStoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestSubStoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestSubStory final : public UGsTable
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
		TMap<FName, const FGsSchemaQuestSubStory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestSubStory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestSubStory*& OutRow) const
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
		const FGsSchemaQuestSubStory* Temp;
		return !FindRowById(FGsSchemaQuestSubStory().id, Temp);
	}
#endif
};
