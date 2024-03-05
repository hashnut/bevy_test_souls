/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipEnchantBalance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipEnchantBalance"))
struct DATACENTER_API FGsSchemaItemEquipEnchantBalance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat1"))
	FGsStatTypeNameSelector stat1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue1"))
	int32 statValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat2"))
	FGsStatTypeNameSelector stat2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue2"))
	int32 statValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stat3"))
	FGsStatTypeNameSelector stat3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue3"))
	int32 statValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stat4"))
	FGsStatTypeNameSelector stat4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue4"))
	int32 statValue4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stat5"))
	FGsStatTypeNameSelector stat5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue5"))
	int32 statValue5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stat6"))
	FGsStatTypeNameSelector stat6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue6"))
	int32 statValue6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stat7"))
	FGsStatTypeNameSelector stat7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue7"))
	int32 statValue7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stat8"))
	FGsStatTypeNameSelector stat8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue8"))
	int32 statValue8;
public:
	FGsSchemaItemEquipEnchantBalance();
	
protected:
	bool operator== (const FGsSchemaItemEquipEnchantBalance& __Other) const
	{
		return FGsSchemaItemEquipEnchantBalance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipEnchantBalance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipEnchantBalance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipEnchantBalanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipEnchantBalanceRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipEnchantBalance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipEnchantBalance* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipEnchantBalance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipEnchantBalanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipEnchantBalanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipEnchantBalance final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
};
