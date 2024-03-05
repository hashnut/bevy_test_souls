/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Map/GsSchemaMapEnterConditionSpotInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Quest/GsSchemaQuest.h"
#include "GsSchemaMapEnterCondition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapEnterCondition"))
struct DATACENTER_API FGsSchemaMapEnterCondition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="QuestId"))
	FGsSchemaQuestRow QuestId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpotList"))
	TArray<FGsSchemaMapEnterConditionSpotInfo> SpotList;
public:
	FGsSchemaMapEnterCondition();
	
protected:
	bool operator== (const FGsSchemaMapEnterCondition& __Other) const
	{
		return FGsSchemaMapEnterCondition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
