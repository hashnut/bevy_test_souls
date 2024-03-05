/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/GsSchemaQuest.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionConditionQuest.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/Condition/BSGsCommunityEventMissionConditionQuest"))
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionQuest
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questId"))
	FGsSchemaQuestRow questId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questType"))
	QuestType questType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatQuestGradeMin"))
	RepeatStoryGrade repeatQuestGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatQuestGradeMax"))
	RepeatStoryGrade repeatQuestGradeMax;
public:
	FGsSchemaCommunityEventMissionConditionQuest();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionConditionQuest& __Other) const
	{
		return FGsSchemaCommunityEventMissionConditionQuest::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionConditionQuest::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionConditionQuest::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionQuestRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionConditionQuestRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionConditionQuest* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionConditionQuest* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionConditionQuest>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionConditionQuestRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionConditionQuestRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionConditionQuest final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission/Condition");
	}
	
};
