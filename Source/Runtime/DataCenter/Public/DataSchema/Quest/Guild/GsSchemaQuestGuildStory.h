/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestGuildStory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Guild/BSGsQuestGuildStory"))
struct DATACENTER_API FGsSchemaQuestGuildStory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	GuildQuestGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minLevel"))
	int32 minLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxLevel"))
	int32 maxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questId"))
	FGsSchemaQuestRow questId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questTitleText"))
	FText questTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questDescriptionText"))
	FText questDescriptionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="zealPointAmount"))
	int32 zealPointAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int32 probability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mainRewardId"))
	FGsSchemaRewardDataRow mainRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subRewardId"))
	FGsSchemaRewardDataRow subRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questIcon"))
	FSoftObjectPath questIcon;
public:
	FGsSchemaQuestGuildStory();
	
protected:
	bool operator== (const FGsSchemaQuestGuildStory& __Other) const
	{
		return FGsSchemaQuestGuildStory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestGuildStory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestGuildStory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestGuildStoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestGuildStoryRow() : Super()
	{
	}
	
	const FGsSchemaQuestGuildStory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestGuildStory* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestGuildStory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestGuildStoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestGuildStoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestGuildStory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Guild");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestGuildStory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestGuildStory>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestGuildStory*& OutRow) const
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
		const FGsSchemaQuestGuildStory* Temp;
		return !FindRowById(FGsSchemaQuestGuildStory().id, Temp);
	}
#endif
};
