/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildAuction.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildStore.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildDailyActivity.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildConfig"))
struct DATACENTER_API FGsSchemaGuildConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="길드 가입 요구레벨", DisplayName="requestUserLevel"))
	int32 requestUserLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="길드 창설 비용", DisplayName="creationCost"))
	int32 creationCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="재가입 시간", DisplayName="reJoinCooldownHour"))
	int32 reJoinCooldownHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="문장 변경 비용", DisplayName="changeEmblemCost"))
	int32 changeEmblemCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기부 보너스 계수", DisplayName="bonusCountFromDonate"))
	float bonusCountFromDonate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="길드 버프 재활성 대기시간(분)", DisplayName="guildBuffReactivateMin"))
	int32 guildBuffReactivateMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="가입 요청할 수 있는 길드 최대 개수", DisplayName="maxJoinRequestCount"))
	int32 maxJoinRequestCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기부 제한 횟수", DisplayName="dailyDonateLimits"))
	int32 dailyDonateLimits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="일일 활동", DisplayName="guildDailyActivityList"))
	TArray<FGsSchemaGuildDailyActivity> guildDailyActivityList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="동맹 대기 기간(분)", DisplayName="allyWaitTimeM"))
	int32 allyWaitTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="전쟁 대기 기간(분)", DisplayName="prepareWarTimeM"))
	int32 prepareWarTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="전쟁 기간(분)", DisplayName="warTimeM"))
	int32 warTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="보호 기간(분)", DisplayName="protectTimeM"))
	int32 protectTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 동맹 요청 수", DisplayName="maxAllySendCount"))
	int32 maxAllySendCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 동맹 요청 받을 수 있는 수", DisplayName="maxAllyReceiveCount"))
	int32 maxAllyReceiveCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 동맹 기사단 수", DisplayName="maxAllyCount"))
	int32 maxAllyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 적대 기사단 수", DisplayName="maxEnemyCount"))
	int32 maxEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="항복 필요 재화량", DisplayName="surrenderCurrencyAmount"))
	int32 surrenderCurrencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="아이템 콜렉션 1인 등록 최대 갯수", DisplayName="itemCollectionRegisterUseLimitCount"))
	int32 itemCollectionRegisterUseLimitCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildStoreList"))
	TArray<FGsSchemaGuildStore> guildStoreList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildAuctionList"))
	TArray<FGsSchemaGuildAuction> guildAuctionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dungeonRestTime"))
	ResetTimeTicket dungeonRestTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dungeonEnterLevel"))
	int32 dungeonEnterLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dungeonTicketLimitCount"))
	int32 dungeonTicketLimitCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dungeonDurationTime"))
	int32 dungeonDurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="newMemberRestrictDungeonForTimeM"))
	int32 newMemberRestrictDungeonForTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonRewardPersonMailTypeId"))
	int32 dungeonRewardPersonMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonRewardJoinMailTypeId"))
	int32 dungeonRewardJoinMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildQuestOpenLevel"))
	int32 guildQuestOpenLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="guildCoinId"))
	FGsSchemaItemCommonRow guildCoinId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildDungeonDeathKickSec"))
	int32 guildDungeonDeathKickSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="newMemberRestrictGuildArenaForTimeM"))
	int32 newMemberRestrictGuildArenaForTimeM;
public:
	FGsSchemaGuildConfig();
	
protected:
	bool operator== (const FGsSchemaGuildConfig& __Other) const
	{
		return FGsSchemaGuildConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildConfigRow() : Super()
	{
	}
	
	const FGsSchemaGuildConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
};
