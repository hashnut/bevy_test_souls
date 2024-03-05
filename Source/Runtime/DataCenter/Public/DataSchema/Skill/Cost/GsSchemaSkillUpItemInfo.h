/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillUpItemInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Cost/BSGsSkillUpItemInfo"))
struct DATACENTER_API FGsSchemaSkillUpItemInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount"))
	int32 itemCount;
public:
	FGsSchemaSkillUpItemInfo();
	
protected:
	bool operator== (const FGsSchemaSkillUpItemInfo& __Other) const
	{
		return FGsSchemaSkillUpItemInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
