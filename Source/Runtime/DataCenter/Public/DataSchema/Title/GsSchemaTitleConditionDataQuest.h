/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTitleConditionDataQuest.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataQuest"))
struct DATACENTER_API FGsSchemaTitleConditionDataQuest
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mMapId"))
	int32 mMapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mRepeatStoryGrade"))
	RepeatStoryGrade mRepeatStoryGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mGuildStoryGrade"))
	GuildQuestGrade mGuildStoryGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questId"))
	int64 questId;
public:
	FGsSchemaTitleConditionDataQuest();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataQuest& __Other) const
	{
		return FGsSchemaTitleConditionDataQuest::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataQuest::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataQuest::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataQuestRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataQuestRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataQuest* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataQuest* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataQuest>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataQuestRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataQuestRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataQuest final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitleConditionDataQuest*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitleConditionDataQuest>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mMapId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaTitleConditionDataQuest*& OutRow) const
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
		const FGsSchemaTitleConditionDataQuest* Temp;
		return !FindRowById(FGsSchemaTitleConditionDataQuest().mMapId, Temp);
	}
#endif
};
