/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/Data/GsItemEnchantShieldRateInfo.h"
#include "GsSchemaItemEquipEnchantShieldRate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipEnchantShieldRate"))
struct DATACENTER_API FGsSchemaItemEquipEnchantShieldRate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="개발자 코멘트"))
	FString desc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probabilityList", Tooltip))
	TArray<FGsItemEnchantShieldRateInfo> probabilityList;
	
public:
	FGsSchemaItemEquipEnchantShieldRate();
	
protected:
	bool operator== (const FGsSchemaItemEquipEnchantShieldRate& __Other) const
	{
		return FGsSchemaItemEquipEnchantShieldRate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipEnchantShieldRate::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipEnchantShieldRate::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipEnchantShieldRateRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipEnchantShieldRateRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipEnchantShieldRate* GetRow() const
	{
		const FGsSchemaItemEquipEnchantShieldRate* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaItemEquipEnchantShieldRate>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipEnchantShieldRateRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipEnchantShieldRateRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipEnchantShieldRate final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEquipEnchantShieldRate*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipEnchantShieldRate>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipEnchantShieldRate*& OutRow) const
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
		const FGsSchemaItemEquipEnchantShieldRate* Temp;
		return !FindRowById(FGsSchemaItemEquipEnchantShieldRate().id, Temp);
	}
#endif
};
