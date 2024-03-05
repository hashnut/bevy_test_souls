/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackConfigDeponMonster.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackConfigDeponMonster"))
struct DATACENTER_API FGsSchemaSpaceCrackConfigDeponMonster
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponType"))
	CreatureWeaponType weaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcId"))
	FGsSchemaNpcDataRow npcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="percentage"))
	int32 percentage;
public:
	FGsSchemaSpaceCrackConfigDeponMonster();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackConfigDeponMonster& __Other) const
	{
		return FGsSchemaSpaceCrackConfigDeponMonster::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
