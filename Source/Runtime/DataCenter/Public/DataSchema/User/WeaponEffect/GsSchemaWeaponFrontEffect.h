/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UParticleSystem;
class UCurveFloat;
#include "GsSchemaWeaponFrontEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/WeaponEffect/BSGsWeaponFrontEffect"))
struct DATACENTER_API FGsSchemaWeaponFrontEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="파티클 이펙트 정보"))
	UParticleSystem* particle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="이펙트 스폰 Offset (Actor Root위치 기준)"))
	FVector rootOffset_L;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="이펙트 스폰 Offset (Actor Root위치 기준)"))
	FVector rootOffset_R;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Dissolve 효과"))
	UCurveFloat* curveDissolve;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Fresnel 효과"))
	UCurveFloat* curveFresnel;
	
public:
	FGsSchemaWeaponFrontEffect();
	
protected:
	bool operator== (const FGsSchemaWeaponFrontEffect& __Other) const
	{
		return FGsSchemaWeaponFrontEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
