/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacySelectCategoryItem.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacySelectGradeItem.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaStarLegacyFilterConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/StarLegacy/BSGsStarLegacyFilterConfig"))
struct DATACENTER_API FGsSchemaStarLegacyFilterConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="selectCategory"))
	TArray<FGsSchemaStarLegacySelectCategoryItem> selectCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="selectGrade"))
	TArray<FGsSchemaStarLegacySelectGradeItem> selectGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="selectEnchantButtonTextFormat"))
	FText selectEnchantButtonTextFormat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="selectEnchant"))
	TArray<int32> selectEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="addSelectCountButtonTextFormat"))
	FText addSelectCountButtonTextFormat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="addSelectCount"))
	TArray<int32> addSelectCount;
public:
	FGsSchemaStarLegacyFilterConfig();
	
protected:
	bool operator== (const FGsSchemaStarLegacyFilterConfig& __Other) const
	{
		return FGsSchemaStarLegacyFilterConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStarLegacyFilterConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStarLegacyFilterConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStarLegacyFilterConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStarLegacyFilterConfigRow() : Super()
	{
	}
	
	const FGsSchemaStarLegacyFilterConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStarLegacyFilterConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaStarLegacyFilterConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStarLegacyFilterConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStarLegacyFilterConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStarLegacyFilterConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("StarLegacy");
	}
	
};
