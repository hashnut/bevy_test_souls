/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "GsSchemaProjectileActionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Projectile/BSGsProjectileActionInfo"))
struct DATACENTER_API FGsSchemaProjectileActionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="최초 틱 오프셋"))
	float tickStartOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="이후 틱 인터벌"))
	float tickInterval;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="발동시킬 SkillNofity"))
	FGsSchemaSkillNotifySetRow skillNotifyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="true : 컬리전 체크 안하고 설정된 틱마다 해당 액션 발동"))
	bool alwaysAct;
public:
	FGsSchemaProjectileActionInfo();
	
protected:
	bool operator== (const FGsSchemaProjectileActionInfo& __Other) const
	{
		return FGsSchemaProjectileActionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
