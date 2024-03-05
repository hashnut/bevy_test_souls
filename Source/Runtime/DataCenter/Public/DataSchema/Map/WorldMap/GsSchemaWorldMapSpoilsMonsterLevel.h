/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMapSpoilsMonsterLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsWorldMapSpoilsMonsterLevel"))
struct DATACENTER_API FGsSchemaWorldMapSpoilsMonsterLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcId"))
	FGsSchemaNpcDataRow npcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isOffensive"))
	bool isOffensive;
public:
	FGsSchemaWorldMapSpoilsMonsterLevel();
	
protected:
	bool operator== (const FGsSchemaWorldMapSpoilsMonsterLevel& __Other) const
	{
		return FGsSchemaWorldMapSpoilsMonsterLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
