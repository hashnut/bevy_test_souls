/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemCraftTab.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/Craft/BSGsItemCraftTab"))
struct DATACENTER_API FGsSchemaItemCraftTab
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCraftTabId"))
	int32 itemCraftTabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="parentItemCraftTabId"))
	int32 parentItemCraftTabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayText"))
	FText displayText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayOrder"))
	int32 displayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="specialTab"))
	EGsCraftSpecialTab specialTab;
public:
	FGsSchemaItemCraftTab();
	
protected:
	bool operator== (const FGsSchemaItemCraftTab& __Other) const
	{
		return FGsSchemaItemCraftTab::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCraftTab::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCraftTab::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCraftTabRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCraftTabRow() : Super()
	{
	}
	
	const FGsSchemaItemCraftTab* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCraftTab* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCraftTab>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCraftTabRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCraftTabRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCraftTab final : public UGsTable
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
		TMap<FName, const FGsSchemaItemCraftTab*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemCraftTab>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemCraftTabId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemCraftTab*& OutRow) const
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
		const FGsSchemaItemCraftTab* Temp;
		return !FindRowById(FGsSchemaItemCraftTab().itemCraftTabId, Temp);
	}
#endif
};
