/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyTalk.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyLevelInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyLevelInfo"))
struct DATACENTER_API FGsSchemaFairyLevelInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	uint8 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="addStatPassivityId"))
	FGsSchemaPassivitySetRow addStatPassivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityIdList"))
	TArray<FGsSchemaPassivitySetRow> passivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="vehiclePassivityId"))
	FGsSchemaPassivitySetRow vehiclePassivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="탈 것의 탑승 연출 효과에 관한 정보를 담은 상태이상 ID", DisplayName="vehicleVisualAbnormality"))
	FGsSchemaAbnormalitySetRow vehicleVisualAbnormality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fairyOnSkillTalk"))
	TArray<FGsSchemaFairyTalk> fairyOnSkillTalk;
public:
	FGsSchemaFairyLevelInfo();
	
protected:
	bool operator== (const FGsSchemaFairyLevelInfo& __Other) const
	{
		return FGsSchemaFairyLevelInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
