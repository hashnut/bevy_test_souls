/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestRepeatScrollRefreshInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Repeat/BSGsQuestRepeatScrollRefreshInfo"))
struct DATACENTER_API FGsSchemaQuestRepeatScrollRefreshInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refreshNumber"))
	int32 refreshNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costCurrencyType"))
	CurrencyType costCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costCurrencyAmount"))
	int64 costCurrencyAmount;
public:
	FGsSchemaQuestRepeatScrollRefreshInfo();
	
protected:
	bool operator== (const FGsSchemaQuestRepeatScrollRefreshInfo& __Other) const
	{
		return FGsSchemaQuestRepeatScrollRefreshInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
