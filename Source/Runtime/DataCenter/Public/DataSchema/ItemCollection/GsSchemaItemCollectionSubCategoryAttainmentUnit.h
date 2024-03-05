/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"
#include "GsSchemaItemCollectionSubCategoryAttainmentUnit.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ItemCollection/BSGsItemCollectionSubCategoryAttainmentUnit"))
struct DATACENTER_API FGsSchemaItemCollectionSubCategoryAttainmentUnit
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemCollectionSubCategoryAttainmentUnit type;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText nameDesc;
public:
	FGsSchemaItemCollectionSubCategoryAttainmentUnit();
	
protected:
	bool operator== (const FGsSchemaItemCollectionSubCategoryAttainmentUnit& __Other) const
	{
		return FGsSchemaItemCollectionSubCategoryAttainmentUnit::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCollectionSubCategoryAttainmentUnit::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCollectionSubCategoryAttainmentUnit::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCollectionSubCategoryAttainmentUnitRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCollectionSubCategoryAttainmentUnitRow() : Super()
	{
	}
	
	const FGsSchemaItemCollectionSubCategoryAttainmentUnit* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCollectionSubCategoryAttainmentUnit* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCollectionSubCategoryAttainmentUnit>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCollectionSubCategoryAttainmentUnitRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCollectionSubCategoryAttainmentUnitRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCollectionSubCategoryAttainmentUnit final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ItemCollection");
	}
	
};
