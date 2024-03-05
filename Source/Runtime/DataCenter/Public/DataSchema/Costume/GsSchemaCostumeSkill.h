/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "GsSchemaCostumeSkill.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeSkill"))
struct DATACENTER_API FGsSchemaCostumeSkill
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponType"))
	CreatureWeaponType weaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="userSkillSetId"))
	FGsSchemaUserSkillSetRow userSkillSetId;
public:
	FGsSchemaCostumeSkill();
	
protected:
	bool operator== (const FGsSchemaCostumeSkill& __Other) const
	{
		return FGsSchemaCostumeSkill::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
