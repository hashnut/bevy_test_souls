/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Level/BSGsSkillNotifyInfo"))
struct DATACENTER_API FGsSchemaSkillNotifyInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="originSkillNotifyId", Tooltip))
	int32 originSkillNotifyId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="lvConvertSkillNotifyId", Tooltip))
	int32 lvConvertSkillNotifyId;
	
public:
	FGsSchemaSkillNotifyInfo();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyInfo& __Other) const
	{
		return FGsSchemaSkillNotifyInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
