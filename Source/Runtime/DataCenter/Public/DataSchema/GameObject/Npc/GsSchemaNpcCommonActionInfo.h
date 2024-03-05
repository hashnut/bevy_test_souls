/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "GsSchemaNpcCommonActionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcCommonActionInfo"))
struct DATACENTER_API FGsSchemaNpcCommonActionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type", Tooltip))
	CommonActionType type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaSkillCommonRow> commonActionId;
	
public:
	FGsSchemaNpcCommonActionInfo();
	
protected:
	bool operator== (const FGsSchemaNpcCommonActionInfo& __Other) const
	{
		return FGsSchemaNpcCommonActionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
