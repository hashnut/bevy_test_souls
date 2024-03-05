/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Icon/GsSchemaQuestImg.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestRepeatStory.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="퀘스트 반복 스토리데이터", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Repeat/BSGsQuestRepeatStory"))
struct DATACENTER_API FGsSchemaQuestRepeatStory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatStoryType"))
	RepeatStoryType repeatStoryType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconRes"))
	FGsSchemaQuestImgRow iconRes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatStoryGrade"))
	RepeatStoryGrade repeatStoryGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int32 probability;
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
public:
	FGsSchemaQuestRepeatStory();
	
protected:
	bool operator== (const FGsSchemaQuestRepeatStory& __Other) const
	{
		return FGsSchemaQuestRepeatStory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestRepeatStory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestRepeatStory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestRepeatStoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestRepeatStoryRow() : Super()
	{
	}
	
	const FGsSchemaQuestRepeatStory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestRepeatStory* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestRepeatStory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestRepeatStoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestRepeatStoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestRepeatStory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Repeat");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestRepeatStory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestRepeatStory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestRepeatStory*& OutRow) const
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
		const FGsSchemaQuestRepeatStory* Temp;
		return !FindRowById(FGsSchemaQuestRepeatStory().id, Temp);
	}
#endif
};
