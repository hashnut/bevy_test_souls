/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaProjectileCollisionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Projectile/BSGsProjectileCollisionInfo"))
struct DATACENTER_API FGsSchemaProjectileCollisionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	SkillCollisionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type == SkillCollisionType::RECT", EditConditionHides, DisplayName="width"))
	int32 width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type == SkillCollisionType::RECT", EditConditionHides, DisplayName="height"))
	int32 height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type != SkillCollisionType::RECT", EditConditionHides, DisplayName="minRadius"))
	int32 minRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type != SkillCollisionType::RECT", EditConditionHides, DisplayName="maxRadius"))
	int32 maxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "type == SkillCollisionType::FAN", EditConditionHides, DisplayName="angle"))
	int32 angle;
public:
	FGsSchemaProjectileCollisionInfo();
	
protected:
	bool operator== (const FGsSchemaProjectileCollisionInfo& __Other) const
	{
		return FGsSchemaProjectileCollisionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
