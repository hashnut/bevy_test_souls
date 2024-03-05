/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWeaponBattleInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/WeaponSocket/BSGsWeaponBattleInfo"))
struct DATACENTER_API FGsSchemaWeaponBattleInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="왼손 Attach 소켓 이름"))
	FName socketName_L;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="오른손 Attach 소켓 이름"))
	FName socketName_R;
	
public:
	FGsSchemaWeaponBattleInfo();
	
protected:
	bool operator== (const FGsSchemaWeaponBattleInfo& __Other) const
	{
		return FGsSchemaWeaponBattleInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
