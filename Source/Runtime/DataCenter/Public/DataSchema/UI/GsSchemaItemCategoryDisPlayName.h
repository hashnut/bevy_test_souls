/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaItemCategoryDisPlayName.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UI/BSGsItemCategoryDisPlayName"))
struct DATACENTER_API FGsSchemaItemCategoryDisPlayName
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySubType"))
	ItemCategorySub itemCategorySubType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayName"))
	FText displayName;
public:
	FGsSchemaItemCategoryDisPlayName();
	
protected:
	bool operator== (const FGsSchemaItemCategoryDisPlayName& __Other) const
	{
		return FGsSchemaItemCategoryDisPlayName::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCategoryDisPlayName::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCategoryDisPlayName::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCategoryDisPlayNameRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCategoryDisPlayNameRow() : Super()
	{
	}
	
	const FGsSchemaItemCategoryDisPlayName* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCategoryDisPlayName* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCategoryDisPlayName>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCategoryDisPlayNameRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCategoryDisPlayNameRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCategoryDisPlayName final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<ItemCategorySub, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("UI");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemCategoryDisPlayName*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemCategoryDisPlayName>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemCategorySubType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const ItemCategorySub& InId, OUT const FGsSchemaItemCategoryDisPlayName*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const ItemCategorySub& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaItemCategoryDisPlayName* Temp;
		return !FindRowById(FGsSchemaItemCategoryDisPlayName().itemCategorySubType, Temp);
	}
#endif
};
