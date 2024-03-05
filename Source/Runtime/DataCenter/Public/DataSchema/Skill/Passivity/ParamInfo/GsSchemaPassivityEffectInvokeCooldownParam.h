/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityCauseInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserSkillSet.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectInvokeCooldownParam.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityEffectInvokeCooldownParam"))
struct DATACENTER_API FGsSchemaPassivityEffectInvokeCooldownParam
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ratePct"))
	int32 ratePct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cause"))
	PassivityCauseType cause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeInfo"))
	FGsSchemaPassivityCauseInfo causeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="target"))
	PassivityInvokeTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="모든 스킬의 쿨다운 타임 조작을 위한 플래그", DisplayName="targetAllUserSkillSetId"))
	bool targetAllUserSkillSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="targetAllUserSkillSetId 플래그 활성 시 해당 데이터는 참조하지 않음", DisplayName="targetUserSkillSetIdList"))
	TArray<FGsSchemaUserSkillSetRow> targetUserSkillSetIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="-3000 10초 짜리 쿨다운일 경우, 30%인 3초가 차감됨. (3초 미만 남은 경우는 남은 만큼만 차감 됨)", DisplayName="targetCooldownRate"))
	int32 targetCooldownRate;
public:
	FGsSchemaPassivityEffectInvokeCooldownParam();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInvokeCooldownParam& __Other) const
	{
		return FGsSchemaPassivityEffectInvokeCooldownParam::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
