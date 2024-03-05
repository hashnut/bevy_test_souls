/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRankingReward.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Ranking/BSGsRankingReward"))
struct DATACENTER_API FGsSchemaRankingReward
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="index"))
	int32 index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankingTypeObjectiveID"))
	FGsSchemaRankingTypeObjectiveRow rankingTypeObjectiveID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardDataList"))
	TArray<FGsSchemaRankingRewardData> rewardDataList;
public:
	FGsSchemaRankingReward();
	
protected:
	bool operator== (const FGsSchemaRankingReward& __Other) const
	{
		return FGsSchemaRankingReward::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRankingReward::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRankingReward::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRankingRewardRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRankingRewardRow() : Super()
	{
	}
	
	const FGsSchemaRankingReward* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRankingReward* Row = nullptr;
		return Table->FindRow<FGsSchemaRankingReward>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRankingRewardRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRankingRewardRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRankingReward final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Ranking");
	}
	
};
