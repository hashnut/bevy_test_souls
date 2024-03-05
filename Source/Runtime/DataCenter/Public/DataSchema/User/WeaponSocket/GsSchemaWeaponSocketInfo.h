/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/User/WeaponSocket/GsSchemaWeaponBattleInfo.h"
#include "DataSchema/User/WeaponSocket/GsSchemaWeaponPeaceInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWeaponSocketInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/WeaponSocket/BSGsWeaponSocketInfo"))
struct DATACENTER_API FGsSchemaWeaponSocketInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="battle", Tooltip))
	FGsSchemaWeaponBattleInfo battle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="peace", Tooltip))
	FGsSchemaWeaponPeaceInfo peace;
	
public:
	FGsSchemaWeaponSocketInfo();
	
protected:
	bool operator== (const FGsSchemaWeaponSocketInfo& __Other) const
	{
		return FGsSchemaWeaponSocketInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
