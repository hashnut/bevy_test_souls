/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityCauseInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityCauseInfo"))
struct DATACENTER_API FGsSchemaPassivityCauseInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	PassivityCauseType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type == PassivityCauseType::UPSERT_ABNORMALITY_TARGET", EditConditionHides, DisplayName="abnormalityEffectType"))
	AbnormalityEffectType abnormalityEffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type == PassivityCauseType::EQUIP_ITEM_SELF", EditConditionHides, DisplayName="itemCategorySub"))
	ItemCategorySub itemCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type == PassivityCauseType::EQUIP_ITEM_SELF", EditConditionHides, DisplayName="itemGrade"))
	ItemGrade itemGrade;
public:
	FGsSchemaPassivityCauseInfo();
	
protected:
	bool operator== (const FGsSchemaPassivityCauseInfo& __Other) const
	{
		return FGsSchemaPassivityCauseInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
