/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "../Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "GsSchemaDropSupplyInfoCreatureIds.generated.h"

struct FGsSchemaWorldMapMonsterElementDataRow;

USTRUCT(BlueprintType, Meta=(Tooltip="CreatureId 배열", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SupplyInfo/BSGsDropSupplyInfoCreatureIds"))
struct DATACENTER_API FGsSchemaDropSupplyInfoCreatureIds
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="WorldmapMonsters", DisplayName="worldmapMonsters"))
	TArray<FGsSchemaWorldMapMonsterElementDataRow> worldmapMonsters;
public:
	FGsSchemaDropSupplyInfoCreatureIds();
	
protected:
	bool operator== (const FGsSchemaDropSupplyInfoCreatureIds& __Other) const
	{
		return FGsSchemaDropSupplyInfoCreatureIds::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
