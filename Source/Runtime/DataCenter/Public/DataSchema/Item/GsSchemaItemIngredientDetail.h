/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Define/EGsIngredientUse_UIType.h"
#include "GsSchemaItemIngredientDetail.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemIngredientDetail"))
struct DATACENTER_API FGsSchemaItemIngredientDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="isUseItem"))
	bool isUseItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="contentsType"))
	EGsIngredientUse_UIType contentsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ingredientDecrement"))
	int32 ingredientDecrement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownGroupId"))
	int32 cooldownGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectType"))
	ItemEffectType effectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectIDList"))
	TArray<int32> effectIDList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tooltipDurationTime"))
	int32 tooltipDurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isAllowedAutoUse"))
	bool isAllowedAutoUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isAllowedAutoSell"))
	bool isAllowedAutoSell;
public:
	FGsSchemaItemIngredientDetail();
	
protected:
	bool operator== (const FGsSchemaItemIngredientDetail& __Other) const
	{
		return FGsSchemaItemIngredientDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemIngredientDetail::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemIngredientDetail::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemIngredientDetailRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemIngredientDetailRow() : Super()
	{
	}
	
	const FGsSchemaItemIngredientDetail* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemIngredientDetail* Row = nullptr;
		return Table->FindRow<FGsSchemaItemIngredientDetail>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemIngredientDetailRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemIngredientDetailRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemIngredientDetail final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemIngredientDetail*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemIngredientDetail>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemIngredientDetail*& OutRow) const
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
		const FGsSchemaItemIngredientDetail* Temp;
		return !FindRowById(FGsSchemaItemIngredientDetail().id, Temp);
	}
#endif
};
