/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardDataBMExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardDataBMExcel"))
struct DATACENTER_API FGsSchemaRewardDataBMExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expMin"))
	int64 expMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expMax"))
	int64 expMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmountMin"))
	int32 currencyAmountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmountMax"))
	int32 currencyAmountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardType"))
	RewardType rewardType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardBoxPairId"))
	int64 rewardBoxPairId;
public:
	FGsSchemaRewardDataBMExcel();
	
protected:
	bool operator== (const FGsSchemaRewardDataBMExcel& __Other) const
	{
		return FGsSchemaRewardDataBMExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardDataBMExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardDataBMExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardDataBMExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardDataBMExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardDataBMExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardDataBMExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardDataBMExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardDataBMExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardDataBMExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardDataBMExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Reward");
	}
	
};
