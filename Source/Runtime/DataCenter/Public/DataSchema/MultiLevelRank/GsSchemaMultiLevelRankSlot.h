/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Rank/GsSchemaQuestMultiLevelRankStory.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMultiLevelRankSlot.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MultiLevelRank/BSGsMultiLevelRankSlot"))
struct DATACENTER_API FGsSchemaMultiLevelRankSlot
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankId"))
	int32 rankId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Name"))
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="NameText"))
	FText NameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subIconPath"))
	FSoftObjectPath subIconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subIconCount"))
	int32 subIconCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityIdList"))
	TArray<FGsSchemaPassivitySetRow> passivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storyId"))
	FGsSchemaQuestMultiLevelRankStoryRow storyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fieldImage"))
	FSoftObjectPath fieldImage;
public:
	FGsSchemaMultiLevelRankSlot();
	
protected:
	bool operator== (const FGsSchemaMultiLevelRankSlot& __Other) const
	{
		return FGsSchemaMultiLevelRankSlot::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMultiLevelRankSlot::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMultiLevelRankSlot::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMultiLevelRankSlotRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMultiLevelRankSlotRow() : Super()
	{
	}
	
	const FGsSchemaMultiLevelRankSlot* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMultiLevelRankSlot* Row = nullptr;
		return Table->FindRow<FGsSchemaMultiLevelRankSlot>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMultiLevelRankSlotRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMultiLevelRankSlotRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMultiLevelRankSlot final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MultiLevelRank");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMultiLevelRankSlot*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMultiLevelRankSlot>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->rankId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaMultiLevelRankSlot*& OutRow) const
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
		const FGsSchemaMultiLevelRankSlot* Temp;
		return !FindRowById(FGsSchemaMultiLevelRankSlot().rankId, Temp);
	}
#endif
};
