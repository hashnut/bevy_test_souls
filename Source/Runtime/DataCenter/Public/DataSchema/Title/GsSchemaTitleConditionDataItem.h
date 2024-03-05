/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaTitleConditionDataItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataItem"))
struct DATACENTER_API FGsSchemaTitleConditionDataItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mItemId"))
	int32 mItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mItemGrade"))
	ItemGrade mItemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mItemType"))
	ItemType mItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCategoryMain"))
	ItemCategoryMain mCategoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mCategorySub"))
	ItemCategorySub mCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mLevel"))
	int32 mLevel;
public:
	FGsSchemaTitleConditionDataItem();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataItem& __Other) const
	{
		return FGsSchemaTitleConditionDataItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataItemRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataItem* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitleConditionDataItem*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitleConditionDataItem>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mItemId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaTitleConditionDataItem*& OutRow) const
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
		const FGsSchemaTitleConditionDataItem* Temp;
		return !FindRowById(FGsSchemaTitleConditionDataItem().mItemId, Temp);
	}
#endif
};
