/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Event/EventAction/GsSchemaEventActionGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogData.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuestObjective.h"
#include "Runtime/DataCenter/Public/DataSchema/SubDialog/GsSchemaSubDialogData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuest.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/BSGsQuest"))
struct DATACENTER_API FGsSchemaQuest
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	QuestType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(MultiLine="true", DisplayName="titleStringId"))
	FText titleStringId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="objectiveProcessType"))
	QuestObjectiveProcessType objectiveProcessType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="objectiveList"))
	TArray<FGsSchemaQuestObjectiveRow> objectiveList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="leavePosInfo"))
	FGsSchemaQuestObjectiveAreaPos leavePosInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTeleportAble"))
	bool isTeleportAble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isAutoPlayAble"))
	bool isAutoPlayAble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="teleportGold"))
	int32 teleportGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isVolatile"))
	bool isVolatile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questContentsType"))
	QuestContentsType questContentsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questBeginSubDialogId"))
	FGsSchemaSubDialogDataRow questBeginSubDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questClearSubDialogId"))
	FGsSchemaSubDialogDataRow questClearSubDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questClearNpcDialogId"))
	FGsSchemaNpcDialogDataRow questClearNpcDialogId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="preQuestEventActionGroupId"))
	FGsSchemaEventActionGroupRow preQuestEventActionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="postQuestEventActionGroupId"))
	FGsSchemaEventActionGroupRow postQuestEventActionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="preEventActionGroupId"))
	int32 preEventActionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="postEventActionGroupId"))
	int32 postEventActionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaQuest();
	
protected:
	bool operator== (const FGsSchemaQuest& __Other) const
	{
		return FGsSchemaQuest::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuest::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuest::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestRow() : Super()
	{
	}
	
	const FGsSchemaQuest* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuest* Row = nullptr;
		return Table->FindRow<FGsSchemaQuest>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuest final : public UGsTable
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
		TMap<FName, const FGsSchemaQuest*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaQuest>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaQuest*& OutRow) const
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
		const FGsSchemaQuest* Temp;
		return !FindRowById(FGsSchemaQuest().id, Temp);
	}
#endif
};
