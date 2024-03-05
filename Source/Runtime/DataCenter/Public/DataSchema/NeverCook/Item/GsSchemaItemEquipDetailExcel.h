/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipDetailExcel.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="ItemEquipDetail Table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Item/BSGsItemEquipDetailExcel"))
struct DATACENTER_API FGsSchemaItemEquipDetailExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="modelPath"))
	int32 modelPath;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType1"))
	FGsStatTypeNameSelector statType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue1"))
	int32 statValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType2"))
	FGsStatTypeNameSelector statType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue2"))
	int32 statValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType3"))
	FGsStatTypeNameSelector statType3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue3"))
	int32 statValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType4"))
	FGsStatTypeNameSelector statType4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue4"))
	int32 statValue4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType5"))
	FGsStatTypeNameSelector statType5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue5"))
	int32 statValue5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType6"))
	FGsStatTypeNameSelector statType6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue6"))
	int32 statValue6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType7"))
	FGsStatTypeNameSelector statType7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue7"))
	int32 statValue7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType8"))
	FGsStatTypeNameSelector statType8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue8"))
	int32 statValue8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemEquipFuseRowName"))
	FName itemEquipFuseRowName;
public:
	FGsSchemaItemEquipDetailExcel();
	
protected:
	bool operator== (const FGsSchemaItemEquipDetailExcel& __Other) const
	{
		return FGsSchemaItemEquipDetailExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipDetailExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipDetailExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipDetailExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipDetailExcelRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipDetailExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipDetailExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipDetailExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipDetailExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipDetailExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipDetailExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEquipDetailExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipDetailExcel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipDetailExcel*& OutRow) const
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
		const FGsSchemaItemEquipDetailExcel* Temp;
		return !FindRowById(FGsSchemaItemEquipDetailExcel().id, Temp);
	}
#endif
};
