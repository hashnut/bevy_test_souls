/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildAuctionMinPrice.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildAuction.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildAuction"))
struct DATACENTER_API FGsSchemaGuildAuction
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 목록 최대 용량", DisplayName="safeCapacity"))
	int32 safeCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 내역 유효기간 (분 단위)", DisplayName="safeStorageTimeMax"))
	int32 safeStorageTimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="경매 수수료 (만 분위)", DisplayName="auctionCommissionRate"))
	int32 auctionCommissionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="경매 목록 최대 용량", DisplayName="auctionCapacity"))
	int32 auctionCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="경매 진행 시간 (분 단위)", DisplayName="auctionOngoing"))
	int32 auctionOngoing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="등급별 경매 등록 최소 금액", DisplayName="auctionRegistration"))
	TArray<FGsSchemaGuildAuctionMinPrice> auctionRegistration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="auctionAutoAddedBid"))
	int32 auctionAutoAddedBid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxPrice"))
	int32 maxPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="storageExpiryHours"))
	int32 storageExpiryHours;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="auctionStandby"))
	int32 auctionStandby;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="auctionBeginMaxPrice"))
	int32 auctionBeginMaxPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mailTypeIdList"))
	TArray<FGsSchemaMailTypeRow> mailTypeIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="multipleAppointmentMax"))
	int32 multipleAppointmentMax;
public:
	FGsSchemaGuildAuction();
	
protected:
	bool operator== (const FGsSchemaGuildAuction& __Other) const
	{
		return FGsSchemaGuildAuction::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
