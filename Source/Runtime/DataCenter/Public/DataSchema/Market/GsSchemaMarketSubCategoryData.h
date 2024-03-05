/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedMarketEnum.h"
#include "GsSchemaMarketSubCategoryData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Market/BSGsMarketSubCategoryData"))
struct DATACENTER_API FGsSchemaMarketSubCategoryData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="marketCategory", Tooltip))
	MarketCategory marketCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="marketCategoryText", Tooltip))
	FText marketCategoryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isAll", Tooltip))
	bool isAll;
public:
	FGsSchemaMarketSubCategoryData();
	
protected:
	bool operator== (const FGsSchemaMarketSubCategoryData& __Other) const
	{
		return FGsSchemaMarketSubCategoryData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
