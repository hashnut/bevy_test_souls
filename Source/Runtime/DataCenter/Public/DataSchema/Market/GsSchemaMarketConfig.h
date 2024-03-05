/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMarketConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Market/BSGsMarketConfig"))
struct DATACENTER_API FGsSchemaMarketConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="등록 수수료(다이아 값에 곱함)", DisplayName="registrationFee"))
	int32 registrationFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최소 판매 금액", DisplayName="minPrice"))
	int32 minPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 판매 금액", DisplayName="maxPrice"))
	int32 maxPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 판매 등록 개수", DisplayName="maxRegistrationCount"))
	int32 maxRegistrationCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최소 등록 대기 시간(분)", DisplayName="minRegistrationWaitingTimeM"))
	int32 minRegistrationWaitingTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 등록 대기 시간(분)", DisplayName="maxRegistrationWaitingTimeM"))
	int32 maxRegistrationWaitingTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="판매 기간(시간)", DisplayName="sellPeriodH"))
	int32 sellPeriodH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 정산 개수", DisplayName="maxSettlementCount"))
	int32 maxSettlementCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="정산 내역 저장 기간(일)", DisplayName="logStorageDurationD"))
	int32 logStorageDurationD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="즐겨찾기 최대 개수", DisplayName="maxBookmarkCount"))
	int32 maxBookmarkCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리스트 요청 시 받을 아이템 개수", DisplayName="listItemCountPerPage"))
	int32 listItemCountPerPage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gmMarketWithdrawMailTypeId"))
	int32 gmMarketWithdrawMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdRegistrationFee"))
	int32 wdRegistrationFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMinPrice"))
	int32 wdMinPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMaxPrice"))
	int32 wdMaxPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMaxRegistrationCount"))
	int32 wdMaxRegistrationCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMinRegistrationWaitingTimeM"))
	int32 wdMinRegistrationWaitingTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMaxRegistrationWaitingTimeM"))
	int32 wdMaxRegistrationWaitingTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdSellPeriodH"))
	int32 wdSellPeriodH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMaxSettlementCount"))
	int32 wdMaxSettlementCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdLogStorageDurationD"))
	int32 wdLogStorageDurationD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMaxBookmarkCount"))
	int32 wdMaxBookmarkCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdListItemCountPerPage"))
	int32 wdListItemCountPerPage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wdMinimumItemGrade"))
	ItemGrade wdMinimumItemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="젠", DisplayName="wdRegistrationFeeItemId"))
	FGsSchemaItemCommonRow wdRegistrationFeeItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이등급부터는 바로가기 시 월드거래소로 보냄", DisplayName="WorldMarketShortcutGrade"))
	ItemGrade WorldMarketShortcutGrade;
public:
	FGsSchemaMarketConfig();
	
protected:
	bool operator== (const FGsSchemaMarketConfig& __Other) const
	{
		return FGsSchemaMarketConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMarketConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMarketConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMarketConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMarketConfigRow() : Super()
	{
	}
	
	const FGsSchemaMarketConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMarketConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaMarketConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMarketConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMarketConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMarketConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Market");
	}
	
};
