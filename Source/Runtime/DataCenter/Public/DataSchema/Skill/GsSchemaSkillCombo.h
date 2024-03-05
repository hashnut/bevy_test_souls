/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillCombo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillCombo"))
struct DATACENTER_API FGsSchemaSkillCombo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nextComboSkillId"))
	int32 nextComboSkillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="currentStage"))
	int32 currentStage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sectionName"))
	FName sectionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetTime"))
	float resetTime;
public:
	FGsSchemaSkillCombo();
	
protected:
	bool operator== (const FGsSchemaSkillCombo& __Other) const
	{
		return FGsSchemaSkillCombo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
