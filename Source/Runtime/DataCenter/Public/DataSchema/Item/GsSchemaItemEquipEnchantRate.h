/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipEnchantRate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipEnchantRate"))
struct DATACENTER_API FGsSchemaItemEquipEnchantRate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantRateId"))
	int32 enchantRateId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="successRate"))
	int32 successRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="failRate"))
	int32 failRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="preserveRate"))
	int32 preserveRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="blessRate"))
	int32 blessRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="curseRate"))
	int32 curseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="downRate"))
	int32 downRate;
public:
	FGsSchemaItemEquipEnchantRate();
	
protected:
	bool operator== (const FGsSchemaItemEquipEnchantRate& __Other) const
	{
		return FGsSchemaItemEquipEnchantRate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipEnchantRate::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipEnchantRate::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipEnchantRateRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipEnchantRateRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipEnchantRate* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipEnchantRate* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipEnchantRate>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipEnchantRateRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipEnchantRateRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipEnchantRate final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
};
