/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventDeliveryBox.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGsContentsEventDeliveryBox"))
struct DATACENTER_API FGsSchemaContentsEventDeliveryBox
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	ContentsEventDeliveryBoxType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minGrade"))
	ItemGrade minGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxGrade"))
	ItemGrade maxGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="category"))
	TArray<ItemCategoryMain> category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categorySub"))
	TArray<ItemCategorySub> categorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionStartTime"))
	FString conditionStartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionEndTime"))
	FString conditionEndTime;
public:
	FGsSchemaContentsEventDeliveryBox();
	
protected:
	bool operator== (const FGsSchemaContentsEventDeliveryBox& __Other) const
	{
		return FGsSchemaContentsEventDeliveryBox::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventDeliveryBox::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventDeliveryBox::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventDeliveryBoxRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventDeliveryBoxRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventDeliveryBox* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventDeliveryBox* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventDeliveryBox>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventDeliveryBoxRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventDeliveryBoxRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventDeliveryBox final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ContentsEvent");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContentsEventDeliveryBox*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventDeliveryBox>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventDeliveryBox*& OutRow) const
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
		const FGsSchemaContentsEventDeliveryBox* Temp;
		return !FindRowById(FGsSchemaContentsEventDeliveryBox().id, Temp);
	}
#endif
};
