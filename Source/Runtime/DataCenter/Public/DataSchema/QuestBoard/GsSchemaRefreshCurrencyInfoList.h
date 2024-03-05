/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRefreshCurrencyInfoList.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/QuestBoard/BSGsRefreshCurrencyInfoList"))
struct DATACENTER_API FGsSchemaRefreshCurrencyInfoList
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RefreshNumber"))
	int32 RefreshNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RefreshCurrencyType"))
	CurrencyType RefreshCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RefreshCurrencyCost"))
	int32 RefreshCurrencyCost;
public:
	FGsSchemaRefreshCurrencyInfoList();
	
protected:
	bool operator== (const FGsSchemaRefreshCurrencyInfoList& __Other) const
	{
		return FGsSchemaRefreshCurrencyInfoList::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
