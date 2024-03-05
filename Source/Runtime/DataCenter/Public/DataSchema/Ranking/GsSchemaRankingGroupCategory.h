/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRankingGroupCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Ranking/BSGsRankingGroupCategory"))
struct DATACENTER_API FGsSchemaRankingGroupCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="index"))
	int32 index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingTypeObjectiveID"))
	FGsSchemaRankingTypeObjectiveRow rankingTypeObjectiveID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankingGroupData"))
	TArray<FGsSchemaRankingGroupData> rankingGroupData;
public:
	FGsSchemaRankingGroupCategory();
	
protected:
	bool operator== (const FGsSchemaRankingGroupCategory& __Other) const
	{
		return FGsSchemaRankingGroupCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRankingGroupCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRankingGroupCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRankingGroupCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRankingGroupCategoryRow() : Super()
	{
	}
	
	const FGsSchemaRankingGroupCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRankingGroupCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaRankingGroupCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRankingGroupCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRankingGroupCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRankingGroupCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Ranking");
	}
	
};
