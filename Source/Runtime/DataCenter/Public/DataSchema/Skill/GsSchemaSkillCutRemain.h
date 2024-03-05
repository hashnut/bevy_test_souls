/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillCutRemain.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillCutRemain"))
struct DATACENTER_API FGsSchemaSkillCutRemain
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float delayOffset;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float forceOffset;
	
public:
	FGsSchemaSkillCutRemain();
	
protected:
	bool operator== (const FGsSchemaSkillCutRemain& __Other) const
	{
		return FGsSchemaSkillCutRemain::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
