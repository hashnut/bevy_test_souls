/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "GsSchemaSkillLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillLevel"))
struct DATACENTER_API FGsSchemaSkillLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 level;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 learnLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaSkillLevelSetRow skillLevelId;
	
public:
	FGsSchemaSkillLevel();
	
protected:
	bool operator== (const FGsSchemaSkillLevel& __Other) const
	{
		return FGsSchemaSkillLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
