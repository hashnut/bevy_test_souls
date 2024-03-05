/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillReaction.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserReactionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BSGsUserReactionInfo"))
struct DATACENTER_API FGsSchemaUserReactionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	EReactionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sword"))
	FGsSchemaSkillReactionRow sword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dagger"))
	FGsSchemaSkillReactionRow dagger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bow"))
	FGsSchemaSkillReactionRow bow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="wand"))
	FGsSchemaSkillReactionRow wand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="orb"))
	FGsSchemaSkillReactionRow orb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shotgun"))
	FGsSchemaSkillReactionRow shotgun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hand"))
	FGsSchemaSkillReactionRow hand;
public:
	FGsSchemaUserReactionInfo();
	
protected:
	bool operator== (const FGsSchemaUserReactionInfo& __Other) const
	{
		return FGsSchemaUserReactionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
