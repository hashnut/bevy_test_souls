/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "GsSchemaNpcReactionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcReactionInfo"))
struct DATACENTER_API FGsSchemaNpcReactionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type", Tooltip))
	EReactionType type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaSkillReactionRow reactionSetId;
public:
	FGsSchemaNpcReactionInfo();
	
protected:
	bool operator== (const FGsSchemaNpcReactionInfo& __Other) const
	{
		return FGsSchemaNpcReactionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
