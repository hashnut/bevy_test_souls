/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectEnchantScroll.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectEnchantScroll"))
struct DATACENTER_API FGsSchemaItemEffectEnchantScroll
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantType"))
	ItemEnchantType enchantType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySub"))
	TArray<ItemCategorySub> itemCategorySub;
public:
	FGsSchemaItemEffectEnchantScroll();
	
protected:
	bool operator== (const FGsSchemaItemEffectEnchantScroll& __Other) const
	{
		return FGsSchemaItemEffectEnchantScroll::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectEnchantScroll::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectEnchantScroll::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectEnchantScrollRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectEnchantScrollRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectEnchantScroll* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectEnchantScroll* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectEnchantScroll>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectEnchantScrollRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectEnchantScrollRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectEnchantScroll final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEffectEnchantScroll*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectEnchantScroll>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectEnchantScroll*& OutRow) const
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
		const FGsSchemaItemEffectEnchantScroll* Temp;
		return !FindRowById(FGsSchemaItemEffectEnchantScroll().effectId, Temp);
	}
#endif
};
