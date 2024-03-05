/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Quest/GsGuideQuestWindowEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestGuideStory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Guide/BSGsQuestGuideStory"))
struct DATACENTER_API FGsSchemaQuestGuideStory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyTitleText"))
	FText storyTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyDescriptionText"))
	FText storyDescriptionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyDetailDescText"))
	FText storyDetailDescText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireLevel"))
	int32 requireLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="requireMainStoryId"))
	FGsSchemaQuestMainStoryRow requireMainStoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questId"))
	FGsSchemaQuestRow questId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mainRewardId"))
	FGsSchemaRewardDataRow mainRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subRewardId"))
	FGsSchemaRewardDataRow subRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guideQuestWindow"))
	EGsGuideQuestWindowType guideQuestWindow;
public:
	FGsSchemaQuestGuideStory();
	
protected:
	bool operator== (const FGsSchemaQuestGuideStory& __Other) const
	{
		return FGsSchemaQuestGuideStory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestGuideStory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestGuideStory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestGuideStoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestGuideStoryRow() : Super()
	{
	}
	
	const FGsSchemaQuestGuideStory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestGuideStory* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestGuideStory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestGuideStoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestGuideStoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestGuideStory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Guide");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestGuideStory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestGuideStory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestGuideStory*& OutRow) const
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
		const FGsSchemaQuestGuideStory* Temp;
		return !FindRowById(FGsSchemaQuestGuideStory().id, Temp);
	}
#endif
};
