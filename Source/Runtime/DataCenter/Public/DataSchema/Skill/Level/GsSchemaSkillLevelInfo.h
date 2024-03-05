/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillLevelInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Level/BSGsSkillLevelInfo"))
struct DATACENTER_API FGsSchemaSkillLevelInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantSkillUpCostId"))
	int32 enchantSkillUpCostId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costConsumeInfo"))
	FGsSchemaSkillCostConsumeInfo costConsumeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="notifyInfoList"))
	TArray<int32> notifyInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectText"))
	FText effectText;
public:
	FGsSchemaSkillLevelInfo();
	
protected:
	bool operator== (const FGsSchemaSkillLevelInfo& __Other) const
	{
		return FGsSchemaSkillLevelInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
