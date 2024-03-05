/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/Collection/EItemCollectionMainCategory.h"
#include "Item/Collection/EItemCollectionFilterType.h"
#include "GsSchemaItemCollectionMainCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ItemCollection/BSGsItemCollectionMainCategory"))
struct DATACENTER_API FGsSchemaItemCollectionMainCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemCollectionMainCategory type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText nameDesc;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemCollectionFilterType FilterType;
public:
	FGsSchemaItemCollectionMainCategory();
	
protected:
	bool operator== (const FGsSchemaItemCollectionMainCategory& __Other) const
	{
		return FGsSchemaItemCollectionMainCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCollectionMainCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCollectionMainCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCollectionMainCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCollectionMainCategoryRow() : Super()
	{
	}
	
	const FGsSchemaItemCollectionMainCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCollectionMainCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCollectionMainCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCollectionMainCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCollectionMainCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCollectionMainCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ItemCollection");
	}
	
};
