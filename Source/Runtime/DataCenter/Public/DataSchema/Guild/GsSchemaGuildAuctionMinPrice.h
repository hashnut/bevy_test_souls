/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildAuctionMinPrice.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildAuctionMinPrice"))
struct DATACENTER_API FGsSchemaGuildAuctionMinPrice
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGrade"))
	ItemGrade itemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minPrice"))
	int32 minPrice;
public:
	FGsSchemaGuildAuctionMinPrice();
	
protected:
	bool operator== (const FGsSchemaGuildAuctionMinPrice& __Other) const
	{
		return FGsSchemaGuildAuctionMinPrice::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
