/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillCostItemInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Level/BSGsSkillCostItemInfo"))
struct DATACENTER_API FGsSchemaSkillCostItemInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId", Tooltip))
	int32 itemId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemAmount", Tooltip))
	int32 itemAmount;
	
public:
	FGsSchemaSkillCostItemInfo();
	
protected:
	bool operator== (const FGsSchemaSkillCostItemInfo& __Other) const
	{
		return FGsSchemaSkillCostItemInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
