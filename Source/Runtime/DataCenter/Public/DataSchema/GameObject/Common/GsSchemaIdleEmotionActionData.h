/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SocialEmotion/GsSchemaIdleEmotionMotionInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIdleEmotionActionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Common/BSGsIdleEmotionActionData"))
struct DATACENTER_API FGsSchemaIdleEmotionActionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="true:순차진행, false:랜덤", DisplayName="useSequence"))
	bool useSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="motionList"))
	TArray<FGsSchemaIdleEmotionMotionInfo> motionList;
public:
	FGsSchemaIdleEmotionActionData();
	
protected:
	bool operator== (const FGsSchemaIdleEmotionActionData& __Other) const
	{
		return FGsSchemaIdleEmotionActionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
