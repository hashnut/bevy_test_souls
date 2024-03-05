/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRankingRewardData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Ranking/BSGsRankingRewardData"))
struct DATACENTER_API FGsSchemaRankingRewardData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingRewardRangeMin"))
	int32 rankingRewardRangeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingRewardRangeMax"))
	int32 rankingRewardRangeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingRewardPassivityID"))
	int32 rankingRewardPassivityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingRewardBadge"))
	FSoftObjectPath rankingRewardBadge;
public:
	FGsSchemaRankingRewardData();
	
protected:
	bool operator== (const FGsSchemaRankingRewardData& __Other) const
	{
		return FGsSchemaRankingRewardData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
