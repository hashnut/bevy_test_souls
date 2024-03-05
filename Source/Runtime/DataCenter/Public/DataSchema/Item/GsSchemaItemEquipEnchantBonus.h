/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaItemEquipEnchantBonus.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipEnchantBonus"))
struct DATACENTER_API FGsSchemaItemEquipEnchantBonus
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="probability"))
	int32 probability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bonusType"))
	EGsItemEnchantBonusType bonusType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openLevel1"))
	int32 openLevel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat1"))
	FGsStatTypeNameSelector stat1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue1"))
	int32 statValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openLevel2"))
	int32 openLevel2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat2"))
	FGsStatTypeNameSelector stat2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue2"))
	int32 statValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openLevel3"))
	int32 openLevel3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat3"))
	FGsStatTypeNameSelector stat3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue3"))
	int32 statValue3;
public:
	FGsSchemaItemEquipEnchantBonus();
	
protected:
	bool operator== (const FGsSchemaItemEquipEnchantBonus& __Other) const
	{
		return FGsSchemaItemEquipEnchantBonus::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipEnchantBonus::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipEnchantBonus::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipEnchantBonusRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipEnchantBonusRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipEnchantBonus* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipEnchantBonus* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipEnchantBonus>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipEnchantBonusRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipEnchantBonusRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipEnchantBonus final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
};
