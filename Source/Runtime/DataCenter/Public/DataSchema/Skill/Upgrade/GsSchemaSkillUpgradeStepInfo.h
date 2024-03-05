/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserSkillSet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillUpgradeStepInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Upgrade/BSGsSkillUpgradeStepInfo"))
struct DATACENTER_API FGsSchemaSkillUpgradeStepInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="step"))
	int32 step;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="requirePassivity"))
	FGsSchemaPassivitySetRow requirePassivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="upgradeSkillId"))
	FGsSchemaUserSkillSetRow upgradeSkillId;
public:
	FGsSchemaSkillUpgradeStepInfo();
	
protected:
	bool operator== (const FGsSchemaSkillUpgradeStepInfo& __Other) const
	{
		return FGsSchemaSkillUpgradeStepInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
