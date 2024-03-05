/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "GsSchemaRankingCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Ranking/BSGsRankingCategory"))
struct DATACENTER_API FGsSchemaRankingCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="index"))
	int32 index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryName"))
	FText categoryName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankingTypeObjective"))
	TArray<FGsSchemaRankingTypeObjectiveRow> rankingTypeObjective;
public:
	FGsSchemaRankingCategory();
	
protected:
	bool operator== (const FGsSchemaRankingCategory& __Other) const
	{
		return FGsSchemaRankingCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRankingCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRankingCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRankingCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRankingCategoryRow() : Super()
	{
	}
	
	const FGsSchemaRankingCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRankingCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaRankingCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRankingCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRankingCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRankingCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Ranking");
	}
	
};
