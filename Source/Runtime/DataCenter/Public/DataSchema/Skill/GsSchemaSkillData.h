/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillData"))
struct DATACENTER_API FGsSchemaSkillData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userSkillSetId", Tooltip))
	int32 userSkillSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillLevel", Tooltip))
	int32 skillLevel;
public:
	FGsSchemaSkillData();
	
protected:
	bool operator== (const FGsSchemaSkillData& __Other) const
	{
		return FGsSchemaSkillData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
