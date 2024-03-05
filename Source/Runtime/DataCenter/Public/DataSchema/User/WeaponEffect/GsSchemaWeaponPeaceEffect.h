/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/User/WeaponEffect/GsSchemaWeaponBackEffect.h"
#include "DataSchema/User/WeaponEffect/GsSchemaWeaponFrontEffect.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWeaponPeaceEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/WeaponEffect/BSGsWeaponPeaceEffect"))
struct DATACENTER_API FGsSchemaWeaponPeaceEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="detachEffect", Tooltip))
	FGsSchemaWeaponFrontEffect detachEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="attachEffect", Tooltip))
	FGsSchemaWeaponBackEffect attachEffect;
	
public:
	FGsSchemaWeaponPeaceEffect();
	
protected:
	bool operator== (const FGsSchemaWeaponPeaceEffect& __Other) const
	{
		return FGsSchemaWeaponPeaceEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
