/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Quest/GsSchemaQuest.h"
#include "Quest/EGsQuestCheckType.h"
#include "GsSchemaSpawnObjectShowHide.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsSpawnObjectShowHide"))
struct DATACENTER_API FGsSchemaSpawnObjectShowHide
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaQuestRow StartConditionQuestId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaQuestRow EndConditionQuestId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestCheckType", Tooltip))
	EGsQuestCheckType QuestCheckType;
public:
	FGsSchemaSpawnObjectShowHide();
	
protected:
	bool operator== (const FGsSchemaSpawnObjectShowHide& __Other) const
	{
		return FGsSchemaSpawnObjectShowHide::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
