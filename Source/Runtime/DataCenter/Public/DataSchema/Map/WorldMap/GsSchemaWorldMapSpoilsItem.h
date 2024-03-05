/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMapSpoilsItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsWorldMapSpoilsItem"))
struct DATACENTER_API FGsSchemaWorldMapSpoilsItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="item"))
	FGsSchemaItemCommonRow item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterLevelArray"))
	TArray<FGsSchemaWorldMapSpoilsMonsterLevel> monsterLevelArray;
public:
	FGsSchemaWorldMapSpoilsItem();
	
protected:
	bool operator== (const FGsSchemaWorldMapSpoilsItem& __Other) const
	{
		return FGsSchemaWorldMapSpoilsItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
