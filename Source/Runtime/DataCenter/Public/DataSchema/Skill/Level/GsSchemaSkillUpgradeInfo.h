/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Upgrade/GsSchemaSkillUpgradeSet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillUpgradeInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Level/BSGsSkillUpgradeInfo"))
struct DATACENTER_API FGsSchemaSkillUpgradeInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currentStep"))
	int32 currentStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillUpgradeId"))
	FGsSchemaSkillUpgradeSetRow skillUpgradeId;
public:
	FGsSchemaSkillUpgradeInfo();
	
protected:
	bool operator== (const FGsSchemaSkillUpgradeInfo& __Other) const
	{
		return FGsSchemaSkillUpgradeInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
