/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "GsSchemaSkillNotifyElementalInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyElementalInfo"))
struct DATACENTER_API FGsSchemaSkillNotifyElementalInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="무속성 대미지"))
	int32 noneDamage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="속성 타입 SharedCombatEnum - DamageElementalType"))
	DamageElementalType elementalType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="속성 대미지"))
	int32 elementalDamage;
	
public:
	FGsSchemaSkillNotifyElementalInfo();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyElementalInfo& __Other) const
	{
		return FGsSchemaSkillNotifyElementalInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
