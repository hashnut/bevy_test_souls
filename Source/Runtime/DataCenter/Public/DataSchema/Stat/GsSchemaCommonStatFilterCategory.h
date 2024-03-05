/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Stat/GsSchemaCommonStatFilterGroup.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommonStatFilterCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsCommonStatFilterCategory"))
struct DATACENTER_API FGsSchemaCommonStatFilterCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryName"))
	FText categoryName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statFilterSet"))
	TArray<FGsSchemaCommonStatFilterGroupRow> statFilterSet;
public:
	FGsSchemaCommonStatFilterCategory();
	
protected:
	bool operator== (const FGsSchemaCommonStatFilterCategory& __Other) const
	{
		return FGsSchemaCommonStatFilterCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommonStatFilterCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommonStatFilterCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommonStatFilterCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommonStatFilterCategoryRow() : Super()
	{
	}
	
	const FGsSchemaCommonStatFilterCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommonStatFilterCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaCommonStatFilterCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommonStatFilterCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommonStatFilterCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommonStatFilterCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Stat");
	}
	
};
