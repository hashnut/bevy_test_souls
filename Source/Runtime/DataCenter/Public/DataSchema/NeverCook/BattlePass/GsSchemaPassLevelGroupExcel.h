/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassLevelGroupExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/BattlePass/BSGsPassLevelGroupExcel"))
struct DATACENTER_API FGsSchemaPassLevelGroupExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passLevelGroupId"))
	int64 passLevelGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exp"))
	int64 exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseCurrencyType"))
	CurrencyType levelPurchaseCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseCurrency"))
	int32 levelPurchaseCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseItemId"))
	int64 levelPurchaseItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelPurchaseItemCount"))
	int32 levelPurchaseItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="basicRewardId"))
	int64 basicRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardId"))
	int64 additionalRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayLevelFlag"))
	bool displayLevelFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="icon"))
	FName icon;
public:
	FGsSchemaPassLevelGroupExcel();
	
protected:
	bool operator== (const FGsSchemaPassLevelGroupExcel& __Other) const
	{
		return FGsSchemaPassLevelGroupExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassLevelGroupExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassLevelGroupExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassLevelGroupExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassLevelGroupExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassLevelGroupExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassLevelGroupExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassLevelGroupExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassLevelGroupExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassLevelGroupExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassLevelGroupExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/BattlePass");
	}
	
};
