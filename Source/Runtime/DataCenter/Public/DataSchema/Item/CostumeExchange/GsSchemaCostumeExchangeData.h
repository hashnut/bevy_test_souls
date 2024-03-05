/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeProbability.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeExchangeData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/CostumeExchange/BSGsCostumeExchangeData"))
struct DATACENTER_API FGsSchemaCostumeExchangeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="recipeGrade"))
	CostumeGrade recipeGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="excludeCostumeIdList"))
	TArray<FGsSchemaCostumeCommonRow> excludeCostumeIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="gainProbabilityInfo"))
	FGsSchemaCostumeProbabilityRow gainProbabilityInfo;
public:
	FGsSchemaCostumeExchangeData();
	
protected:
	bool operator== (const FGsSchemaCostumeExchangeData& __Other) const
	{
		return FGsSchemaCostumeExchangeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
