/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GameObject/Define/EGsProjectileApplyTargetType.h"
#include "GsSchemaScreenFilterInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Common/BSGsScreenFilterInfo"))
struct DATACENTER_API FGsSchemaScreenFilterInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="screenFilterUse"))
	bool screenFilterUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="applyTarget", Tooltip="나와 적대적인 대상에게만 적용"))
	EGsProjectileApplyTargetType applyTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="uiName"))
	FName uiName;
public:
	FGsSchemaScreenFilterInfo();
	
protected:
	bool operator== (const FGsSchemaScreenFilterInfo& __Other) const
	{
		return FGsSchemaScreenFilterInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
