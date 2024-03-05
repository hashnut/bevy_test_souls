/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsSchemaExtendSlotCost.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsExtendSlotCost"))
struct DATACENTER_API FGsSchemaExtendSlotCost
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendNumber", Tooltip))
	int32 extendNumber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCurrencyType", Tooltip))
	CurrencyType extendSlotCurrencyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCurrencyCost", Tooltip))
	int32 extendSlotCurrencyCost;
	
public:
	FGsSchemaExtendSlotCost();
	
protected:
	bool operator== (const FGsSchemaExtendSlotCost& __Other) const
	{
		return FGsSchemaExtendSlotCost::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
