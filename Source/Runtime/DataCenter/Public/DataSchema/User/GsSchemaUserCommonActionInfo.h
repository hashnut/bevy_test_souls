/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserCommonActionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BsGsUserCommonActionInfo"))
struct DATACENTER_API FGsSchemaUserCommonActionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	CommonActionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sword"))
	FGsSchemaSkillCommonRow sword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dagger"))
	FGsSchemaSkillCommonRow dagger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bow"))
	FGsSchemaSkillCommonRow bow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="wand"))
	FGsSchemaSkillCommonRow wand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="orb"))
	FGsSchemaSkillCommonRow orb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="shotgun"))
	FGsSchemaSkillCommonRow shotgun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hand"))
	FGsSchemaSkillCommonRow hand;
public:
	FGsSchemaUserCommonActionInfo();
	
protected:
	bool operator== (const FGsSchemaUserCommonActionInfo& __Other) const
	{
		return FGsSchemaUserCommonActionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
