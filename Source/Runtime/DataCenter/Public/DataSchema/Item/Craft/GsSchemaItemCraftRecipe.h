/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaItemCraftRecipe.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/Craft/BSGsItemCraftRecipe"))
struct DATACENTER_API FGsSchemaItemCraftRecipe
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCraftRecipeId"))
	int32 itemCraftRecipeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCraftTabId"))
	int32 itemCraftTabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="recipeText"))
	FText recipeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayOrder"))
	int32 displayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount"))
	int32 currencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardDataId"))
	int32 rewardDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="unlockCondition"))
	ItemCraftUnlockType unlockCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="unlockValue"))
	int32 unlockValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material1"))
	int32 material1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material1Amount"))
	int32 material1Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material2"))
	int32 material2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material2Amount"))
	int32 material2Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material3"))
	int32 material3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material3Amount"))
	int32 material3Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material4"))
	int32 material4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material4Amount"))
	int32 material4Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material5"))
	int32 material5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material5Amount"))
	int32 material5Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material6"))
	int32 material6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material6Amount"))
	int32 material6Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material7"))
	int32 material7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material7Amount"))
	int32 material7Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material8"))
	int32 material8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material8Amount"))
	int32 material8Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material9"))
	int32 material9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material9Amount"))
	int32 material9Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material10"))
	int32 material10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="material10Amount"))
	int32 material10Amount;
public:
	FGsSchemaItemCraftRecipe();
	
protected:
	bool operator== (const FGsSchemaItemCraftRecipe& __Other) const
	{
		return FGsSchemaItemCraftRecipe::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCraftRecipe::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCraftRecipe::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCraftRecipeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCraftRecipeRow() : Super()
	{
	}
	
	const FGsSchemaItemCraftRecipe* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCraftRecipe* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCraftRecipe>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCraftRecipeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCraftRecipeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCraftRecipe final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item/Craft");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemCraftRecipe*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemCraftRecipe>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemCraftRecipeId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemCraftRecipe*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaItemCraftRecipe* Temp;
		return !FindRowById(FGsSchemaItemCraftRecipe().itemCraftRecipeId, Temp);
	}
#endif
};
