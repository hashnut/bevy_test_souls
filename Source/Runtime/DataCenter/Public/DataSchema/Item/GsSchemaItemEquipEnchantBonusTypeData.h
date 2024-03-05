/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsSchemaItemEquipEnchantBonusTypeData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipEnchantBonusTypeData"))
struct DATACENTER_API FGsSchemaItemEquipEnchantBonusTypeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantBonusType"))
	EGsItemEnchantBonusType enchantBonusType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
public:
	FGsSchemaItemEquipEnchantBonusTypeData();
	
protected:
	bool operator== (const FGsSchemaItemEquipEnchantBonusTypeData& __Other) const
	{
		return FGsSchemaItemEquipEnchantBonusTypeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipEnchantBonusTypeData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipEnchantBonusTypeData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipEnchantBonusTypeDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipEnchantBonusTypeDataRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipEnchantBonusTypeData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipEnchantBonusTypeData* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipEnchantBonusTypeData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipEnchantBonusTypeDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipEnchantBonusTypeDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipEnchantBonusTypeData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsItemEnchantBonusType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEquipEnchantBonusTypeData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipEnchantBonusTypeData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->enchantBonusType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsItemEnchantBonusType& InId, OUT const FGsSchemaItemEquipEnchantBonusTypeData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsItemEnchantBonusType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaItemEquipEnchantBonusTypeData* Temp;
		return !FindRowById(FGsSchemaItemEquipEnchantBonusTypeData().enchantBonusType, Temp);
	}
#endif
};
