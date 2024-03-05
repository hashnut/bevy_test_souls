/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Repeat/GsSchemaQuestRepeatScrollRefreshInfoSet.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestRepeatScroll.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Repeat/BSGsQuestRepeatScroll"))
struct DATACENTER_API FGsSchemaQuestRepeatScroll
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="chapterTitleText"))
	FText chapterTitleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questStoryList"))
	TArray<FGsSchemaQuestRepeatStoryRow> questStoryList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="refreshInfoSet"))
	FGsSchemaQuestRepeatScrollRefreshInfoSetRow refreshInfoSet;
public:
	FGsSchemaQuestRepeatScroll();
	
protected:
	bool operator== (const FGsSchemaQuestRepeatScroll& __Other) const
	{
		return FGsSchemaQuestRepeatScroll::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestRepeatScroll::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestRepeatScroll::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestRepeatScrollRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestRepeatScrollRow() : Super()
	{
	}
	
	const FGsSchemaQuestRepeatScroll* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestRepeatScroll* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestRepeatScroll>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestRepeatScrollRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestRepeatScrollRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestRepeatScroll final : public UGsTable
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
		TMap<FName, const FGsSchemaQuestRepeatScroll*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestRepeatScroll>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestRepeatScroll*& OutRow) const
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
		const FGsSchemaQuestRepeatScroll* Temp;
		return !FindRowById(FGsSchemaQuestRepeatScroll().id, Temp);
	}
#endif
};
