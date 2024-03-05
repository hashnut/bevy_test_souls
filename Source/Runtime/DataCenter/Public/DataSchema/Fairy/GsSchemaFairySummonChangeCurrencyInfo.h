/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairySummonChangeCurrencyInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairySummonChangeCurrencyInfo"))
struct DATACENTER_API FGsSchemaFairySummonChangeCurrencyInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="changeCount"))
	uint8 changeCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value"))
	int32 value;
public:
	FGsSchemaFairySummonChangeCurrencyInfo();
	
protected:
	bool operator== (const FGsSchemaFairySummonChangeCurrencyInfo& __Other) const
	{
		return FGsSchemaFairySummonChangeCurrencyInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
