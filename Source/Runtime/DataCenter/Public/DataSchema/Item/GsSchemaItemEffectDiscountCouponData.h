/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectDiscountCouponData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectDiscountCouponData"))
struct DATACENTER_API FGsSchemaItemEffectDiscountCouponData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	ItemCouponDiscountType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionGroupId"))
	int32 conditionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="constantValue"))
	int32 constantValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="coefficientValue"))
	int32 coefficientValue;
public:
	FGsSchemaItemEffectDiscountCouponData();
	
protected:
	bool operator== (const FGsSchemaItemEffectDiscountCouponData& __Other) const
	{
		return FGsSchemaItemEffectDiscountCouponData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectDiscountCouponData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectDiscountCouponData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectDiscountCouponDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectDiscountCouponDataRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectDiscountCouponData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectDiscountCouponData* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectDiscountCouponData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectDiscountCouponDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectDiscountCouponDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectDiscountCouponData final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEffectDiscountCouponData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectDiscountCouponData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectDiscountCouponData*& OutRow) const
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
		const FGsSchemaItemEffectDiscountCouponData* Temp;
		return !FindRowById(FGsSchemaItemEffectDiscountCouponData().id, Temp);
	}
#endif
};
