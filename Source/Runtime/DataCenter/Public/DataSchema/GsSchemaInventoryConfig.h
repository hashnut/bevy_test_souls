/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInventoryConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsInventoryConfig"))
struct DATACENTER_API FGsSchemaInventoryConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCount"))
	int32 extendSlotCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendMaxCount"))
	int32 extendMaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCurrencyType"))
	CurrencyType extendSlotCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCurrencyCost"))
	int32 extendSlotCurrencyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultSlotCount"))
	int32 defaultSlotCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weightPenalty1Phase"))
	int32 weightPenalty1Phase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weightPenalty2Phase"))
	int32 weightPenalty2Phase;
public:
	FGsSchemaInventoryConfig();
	
protected:
	bool operator== (const FGsSchemaInventoryConfig& __Other) const
	{
		return FGsSchemaInventoryConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInventoryConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInventoryConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInventoryConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInventoryConfigRow() : Super()
	{
	}
	
	const FGsSchemaInventoryConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInventoryConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaInventoryConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInventoryConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInventoryConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInventoryConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
};
