/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "GsSchemaAchievementConditionQuest.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/AchievementCondition/BSGsAchievementConditionQuest"))
struct DATACENTER_API FGsSchemaAchievementConditionQuest
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questType"))
	QuestType questType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatStoryGrade"))
	RepeatStoryGrade repeatStoryGrade;
public:
	FGsSchemaAchievementConditionQuest();
	
protected:
	bool operator== (const FGsSchemaAchievementConditionQuest& __Other) const
	{
		return FGsSchemaAchievementConditionQuest::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementConditionQuest::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementConditionQuest::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementConditionQuestRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementConditionQuestRow() : Super()
	{
	}
	
	const FGsSchemaAchievementConditionQuest* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementConditionQuest* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementConditionQuest>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementConditionQuestRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementConditionQuestRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementConditionQuest final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement/AchievementCondition");
	}
	
};
