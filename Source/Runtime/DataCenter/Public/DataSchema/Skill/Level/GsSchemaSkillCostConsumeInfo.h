/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Skill/Level/GsSchemaSkillStatInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostItemInfo.h"
#include "GsSchemaSkillCostConsumeInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Level/BSGsSkillCostConsumeInfo"))
struct DATACENTER_API FGsSchemaSkillCostConsumeInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="requireItemList", Tooltip="소모되는 아이템의 id와 수량"))
	TArray<FGsSchemaSkillCostItemInfo> requireItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statInfoList", Tooltip="소모되는 스텟 정보, HP와 MP가 동시에 소모될 수 있음"))
	TArray<FGsSchemaSkillStatInfo> statInfoList;
public:
	FGsSchemaSkillCostConsumeInfo();
	
protected:
	bool operator== (const FGsSchemaSkillCostConsumeInfo& __Other) const
	{
		return FGsSchemaSkillCostConsumeInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
