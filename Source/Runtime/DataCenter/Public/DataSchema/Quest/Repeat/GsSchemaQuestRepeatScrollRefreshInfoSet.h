/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Repeat/GsSchemaQuestRepeatScrollRefreshInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestRepeatScrollRefreshInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Repeat/BSGsQuestRepeatScrollRefreshInfoSet"))
struct DATACENTER_API FGsSchemaQuestRepeatScrollRefreshInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyInfoList"))
	TArray<FGsSchemaQuestRepeatScrollRefreshInfo> currencyInfoList;
public:
	FGsSchemaQuestRepeatScrollRefreshInfoSet();
	
protected:
	bool operator== (const FGsSchemaQuestRepeatScrollRefreshInfoSet& __Other) const
	{
		return FGsSchemaQuestRepeatScrollRefreshInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestRepeatScrollRefreshInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestRepeatScrollRefreshInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestRepeatScrollRefreshInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestRepeatScrollRefreshInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaQuestRepeatScrollRefreshInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestRepeatScrollRefreshInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestRepeatScrollRefreshInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestRepeatScrollRefreshInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestRepeatScrollRefreshInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestRepeatScrollRefreshInfoSet final : public UGsTable
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
		TMap<FName, const FGsSchemaQuestRepeatScrollRefreshInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestRepeatScrollRefreshInfoSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaQuestRepeatScrollRefreshInfoSet*& OutRow) const
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
		const FGsSchemaQuestRepeatScrollRefreshInfoSet* Temp;
		return !FindRowById(FGsSchemaQuestRepeatScrollRefreshInfoSet().id, Temp);
	}
#endif
};
