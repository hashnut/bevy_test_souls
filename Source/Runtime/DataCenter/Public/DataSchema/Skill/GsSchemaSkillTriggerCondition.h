/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillExecuteConditionSet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillTriggerCondition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillTriggerCondition"))
struct DATACENTER_API FGsSchemaSkillTriggerCondition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 발동 조건 데이터", DisplayName="executeConditionId"))
	FGsSchemaSkillExecuteConditionSetRow executeConditionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 시전자의 스킬 사용 불가 행동 제약에 관계없이 사용해야할 경우 true 로 설정", DisplayName="ignoreCreatureBehavior"))
	bool ignoreCreatureBehavior;
public:
	FGsSchemaSkillTriggerCondition();
	
protected:
	bool operator== (const FGsSchemaSkillTriggerCondition& __Other) const
	{
		return FGsSchemaSkillTriggerCondition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
