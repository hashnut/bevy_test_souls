/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaEnchantLevelList.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemDecompose.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemDecompose"))
struct DATACENTER_API FGsSchemaItemDecompose
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="decomposeType"))
	ItemDecomposeType decomposeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemIdList"))
	TArray<FGsSchemaItemCommonRow> itemIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategoryMain"))
	ItemCategoryMain itemCategoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantLevelList"))
	TArray<FGsSchemaEnchantLevelList> enchantLevelList;
public:
	FGsSchemaItemDecompose();
	
protected:
	bool operator== (const FGsSchemaItemDecompose& __Other) const
	{
		return FGsSchemaItemDecompose::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemDecompose::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemDecompose::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemDecomposeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemDecomposeRow() : Super()
	{
	}
	
	const FGsSchemaItemDecompose* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemDecompose* Row = nullptr;
		return Table->FindRow<FGsSchemaItemDecompose>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemDecomposeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemDecomposeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemDecompose final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
};
