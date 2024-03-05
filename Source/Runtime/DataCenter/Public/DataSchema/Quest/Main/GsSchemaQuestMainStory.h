/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Event/EventAction/GsSchemaEventActionGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/SubDialog/GsSchemaSubDialogData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestMainStory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Main/BSGsQuestMainStory"))
struct DATACENTER_API FGsSchemaQuestMainStory
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyBeginSubDialogId"))
	FGsSchemaSubDialogDataRow storyBeginSubDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyClearSubDialogId"))
	FGsSchemaSubDialogDataRow storyClearSubDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="postStoryEventActionGroupId"))
	FGsSchemaEventActionGroupRow postStoryEventActionGroupId;
public:
	FGsSchemaQuestMainStory();
	
protected:
	bool operator== (const FGsSchemaQuestMainStory& __Other) const
	{
		return FGsSchemaQuestMainStory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestMainStory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestMainStory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestMainStoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestMainStoryRow() : Super()
	{
	}
	
	const FGsSchemaQuestMainStory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestMainStory* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestMainStory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestMainStoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestMainStoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestMainStory final : public UGsTable
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
		TMap<FName, const FGsSchemaQuestMainStory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestMainStory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestMainStory*& OutRow) const
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
		const FGsSchemaQuestMainStory* Temp;
		return !FindRowById(FGsSchemaQuestMainStory().id, Temp);
	}
#endif
};
