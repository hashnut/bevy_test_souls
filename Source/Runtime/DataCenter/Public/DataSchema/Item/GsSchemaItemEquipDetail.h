/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Item/Data/GsItemStatData.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemEquipFuse.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipDetail.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="ItemEquipDetail Table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipDetail"))
struct DATACENTER_API FGsSchemaItemEquipDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="modelPath"))
	int32 modelPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statList"))
	TArray<FGsItemStatData> statList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="materialType"))
	ItemMaterial materialType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponType"))
	CreatureWeaponType weaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="safeEnchantLevel"))
	int32 safeEnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantShieldId"))
	int32 enchantShieldId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantRateId"))
	int32 enchantRateId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantBonusId"))
	int32 enchantBonusId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantBalanceId"))
	int32 enchantBalanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantCost"))
	int32 enchantCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityIdList"))
	TArray<int32> passivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineOptionCostCurrencyType"))
	CurrencyType refineOptionCostCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineOptionCost"))
	int32 refineOptionCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineOptionId"))
	TArray<int32> refineOptionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineBonusOptionIds"))
	TArray<int32> refineBonusOptionIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="refineBonusOptionProbs"))
	TArray<int32> refineBonusOptionProbs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="goldRecoveryCost"))
	int32 goldRecoveryCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="diaRecoveryCost"))
	int32 diaRecoveryCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="setId"))
	int32 setId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemOptionSkillGroupId"))
	int32 itemOptionSkillGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipGroupId"))
	int32 equipGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemFusionData"))
	FGsSchemaItemEquipFuseRow itemFusionData;
public:
	FGsSchemaItemEquipDetail();
	
protected:
	bool operator== (const FGsSchemaItemEquipDetail& __Other) const
	{
		return FGsSchemaItemEquipDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipDetail::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipDetail::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipDetailRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipDetailRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipDetail* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipDetail* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipDetail>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipDetailRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipDetailRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipDetail final : public UGsTable
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
		TMap<FName, const FGsSchemaItemEquipDetail*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipDetail>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipDetail*& OutRow) const
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
		const FGsSchemaItemEquipDetail* Temp;
		return !FindRowById(FGsSchemaItemEquipDetail().id, Temp);
	}
#endif
};
