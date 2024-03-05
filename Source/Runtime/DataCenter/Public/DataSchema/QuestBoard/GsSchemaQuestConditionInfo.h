/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/QuestBoard/GsSchemaQuestRepeatScrollList.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestConditionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/QuestBoard/BSGsQuestConditionInfo"))
struct DATACENTER_API FGsSchemaQuestConditionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="QuestMainStroy"))
	FGsSchemaQuestMainStoryRow QuestMainStroy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestBoardType"))
	QuestBoardType QuestBoardType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestRepeatScrollList"))
	TArray<FGsSchemaQuestRepeatScrollList> QuestRepeatScrollList;
public:
	FGsSchemaQuestConditionInfo();
	
protected:
	bool operator== (const FGsSchemaQuestConditionInfo& __Other) const
	{
		return FGsSchemaQuestConditionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
