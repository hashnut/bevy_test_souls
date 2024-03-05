/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UCurveVector;
#include "GsSchemaWeaponPeaceInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/WeaponSocket/BSGsWeaponPeaceInfo"))
struct DATACENTER_API FGsSchemaWeaponPeaceInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Attach 소켓 이름", DisplayName="socketName"))
	FName socketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="왼손 무기 Offset 정보", DisplayName="socketOffset_L"))
	FTransform socketOffset_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="오른손 무기 Offset 정보", DisplayName="socketOffset_R"))
	FTransform socketOffset_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UpdateLocation"))
	UCurveVector* UpdateLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UpdateRotation"))
	UCurveVector* UpdateRotation;
public:
	FGsSchemaWeaponPeaceInfo();
	
protected:
	bool operator== (const FGsSchemaWeaponPeaceInfo& __Other) const
	{
		return FGsSchemaWeaponPeaceInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
