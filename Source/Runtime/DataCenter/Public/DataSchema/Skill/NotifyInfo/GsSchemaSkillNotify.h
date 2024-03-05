/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "GsSchemaSkillNotify.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotify"))
struct DATACENTER_API FGsSchemaSkillNotify
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaSkillNotifySetRow skillNotifyId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float offset;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 repeatCount;
	
public:
	FGsSchemaSkillNotify();
	
protected:
	bool operator== (const FGsSchemaSkillNotify& __Other) const
	{
		return FGsSchemaSkillNotify::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
