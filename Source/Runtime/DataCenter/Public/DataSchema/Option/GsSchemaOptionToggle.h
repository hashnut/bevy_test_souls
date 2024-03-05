/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionToggle.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionToggle"))
struct DATACENTER_API FGsSchemaOptionToggle
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="optionItemType"))
	EGsOptionItem optionItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="onText"))
	FText onText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="offText"))
	FText offText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionExceptionPlatformTypeList"))
	TArray<EGsOptionPlatformType> OptionExceptionPlatformTypeList;
public:
	FGsSchemaOptionToggle();
	
protected:
	bool operator== (const FGsSchemaOptionToggle& __Other) const
	{
		return FGsSchemaOptionToggle::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
