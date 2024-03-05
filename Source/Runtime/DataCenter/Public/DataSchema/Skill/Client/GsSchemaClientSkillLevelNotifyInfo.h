/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "GsSchemaClientSkillLevelNotifyInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Client/BSGsClientSkillLevelNotifyInfo"))
struct DATACENTER_API FGsSchemaClientSkillLevelNotifyInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="skillNotify"))
	FGsSchemaSkillNotifySetRow skillNotify;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="offset"))
	float offset;
public:
	FGsSchemaClientSkillLevelNotifyInfo();
	
protected:
	bool operator== (const FGsSchemaClientSkillLevelNotifyInfo& __Other) const
	{
		return FGsSchemaClientSkillLevelNotifyInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
