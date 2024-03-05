/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIdleEmotionMotionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SocialEmotion/BSGsIdleEmotionMotionInfo"))
struct DATACENTER_API FGsSchemaIdleEmotionMotionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="emotion"))
	UserSocialEmotion emotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="motion"))
	FGsSchemaSkillCommonRow motion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="randRate"))
	float randRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="useIdleMotion"))
	bool useIdleMotion;
public:
	FGsSchemaIdleEmotionMotionInfo();
	
protected:
	bool operator== (const FGsSchemaIdleEmotionMotionInfo& __Other) const
	{
		return FGsSchemaIdleEmotionMotionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
