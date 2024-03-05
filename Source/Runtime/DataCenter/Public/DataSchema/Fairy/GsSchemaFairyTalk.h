/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyTalk.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyTalk"))
struct DATACENTER_API FGsSchemaFairyTalk
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="talkOnType"))
	EGsFairyTalkType talkOnType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="onSkillTalk"))
	FText onSkillTalk;
public:
	FGsSchemaFairyTalk();
	
protected:
	bool operator== (const FGsSchemaFairyTalk& __Other) const
	{
		return FGsSchemaFairyTalk::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
