/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "Runtime/DataCenter/Public/DataSchema/SubDialog/GsSchemaSubDialogData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestObjective.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsQuestObjective"))
struct DATACENTER_API FGsSchemaQuestObjective
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MultiLine="true", DisplayName="descStringId"))
	FText descStringId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	QuestObjectiveType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="posList"))
	TArray<FGsSchemaQuestObjectiveAreaPos> posList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="typeInfo"))
	FGsSchemaRowPoly typeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="objectiveClearSubDialogId"))
	FGsSchemaSubDialogDataRow objectiveClearSubDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTickerHide"))
	bool isTickerHide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isCountHide"))
	bool isCountHide;
public:
	FGsSchemaQuestObjective();
	
protected:
	bool operator== (const FGsSchemaQuestObjective& __Other) const
	{
		return FGsSchemaQuestObjective::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestObjective::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestObjective::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestObjectiveRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestObjectiveRow() : Super()
	{
	}
	
	const FGsSchemaQuestObjective* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestObjective* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestObjective>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestObjectiveRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestObjectiveRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestObjective final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaQuestObjective*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuestObjective>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaQuestObjective*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaQuestObjective* Temp;
		return !FindRowById(FGsSchemaQuestObjective().id, Temp);
	}
#endif
};
