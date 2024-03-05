/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "Item/Collection/EItemCollectionSubCategoryStatUnit.h"
#include "GsSchemaItemCollectionSubCategoryStatUnit.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ItemCollection/BSGsItemCollectionSubCategoryStatUnit"))
struct DATACENTER_API FGsSchemaItemCollectionSubCategoryStatUnit
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	EItemCollectionSubCategoryStatUnit type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameDesc"))
	FText nameDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statTypes"))
	TSet<FGsStatTypeNameSelector> statTypes;
public:
	FGsSchemaItemCollectionSubCategoryStatUnit();
	
protected:
	bool operator== (const FGsSchemaItemCollectionSubCategoryStatUnit& __Other) const
	{
		return FGsSchemaItemCollectionSubCategoryStatUnit::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCollectionSubCategoryStatUnit::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCollectionSubCategoryStatUnit::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCollectionSubCategoryStatUnitRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCollectionSubCategoryStatUnitRow() : Super()
	{
	}
	
	const FGsSchemaItemCollectionSubCategoryStatUnit* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCollectionSubCategoryStatUnit* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCollectionSubCategoryStatUnit>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCollectionSubCategoryStatUnitRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCollectionSubCategoryStatUnitRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCollectionSubCategoryStatUnit final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ItemCollection");
	}
	
};
