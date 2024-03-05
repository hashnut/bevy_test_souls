/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/PersonalTrading/GsSchemaPersonalTradingCharge.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPersonalTradingConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/PersonalTrading/BSGsPersonalTradingConfig"))
struct DATACENTER_API FGsSchemaPersonalTradingConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이용 가능 레벨", DisplayName="tradeOpenLevel"))
	int32 tradeOpenLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="개인 거래 요청 승인 시간(초)", DisplayName="approveProgressTimeSec"))
	int32 approveProgressTimeSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="일일 거래 가능 횟수", DisplayName="dailyTradeNumber"))
	int32 dailyTradeNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tradeCharge"))
	FGsSchemaPersonalTradingCharge tradeCharge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="거래 슬롯 수", DisplayName="slotCnt"))
	int32 slotCnt;
public:
	FGsSchemaPersonalTradingConfig();
	
protected:
	bool operator== (const FGsSchemaPersonalTradingConfig& __Other) const
	{
		return FGsSchemaPersonalTradingConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPersonalTradingConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPersonalTradingConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPersonalTradingConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPersonalTradingConfigRow() : Super()
	{
	}
	
	const FGsSchemaPersonalTradingConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPersonalTradingConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaPersonalTradingConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPersonalTradingConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPersonalTradingConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePersonalTradingConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("PersonalTrading");
	}
	
};
