/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailType.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAgitDungeonRaidConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/Agit/BSGsAgitDungeonRaidConfig"))
struct DATACENTER_API FGsSchemaAgitDungeonRaidConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공략던전 가능 기사단 레벨", DisplayName="requiredGuildLevel"))
	int32 requiredGuildLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공략던전 최대 완료 횟수", DisplayName="raidMaxClearCount"))
	int32 raidMaxClearCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공략던전 최대 참여 횟수_계정종속", DisplayName="raidTicketCount"))
	int32 raidTicketCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공략던전 이용시간_분", DisplayName="raidDurationTimeM"))
	int32 raidDurationTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공략던전 신규 가입자 참여 제한시간_분", DisplayName="newMemberRestrictTimeM"))
	int32 newMemberRestrictTimeM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공략던전 개별 보상 우편 타입", DisplayName="raidRewardPersonMailTypeId"))
	FGsSchemaMailTypeRow raidRewardPersonMailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 시작 가능한 단원 최소 등급", DisplayName="raidOpenMinGrade"))
	GuildMemberGradeType raidOpenMinGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ShowAllItems, Tooltip="시작 연출 티커 아이콘 경로", DisplayName="tickerIconPathStart"))
	FSoftObjectPath tickerIconPathStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성공 연출 티커 아이콘 경로", DisplayName="tickerIconPathClear"))
	FSoftObjectPath tickerIconPathClear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="시작 연출 티커 사운드", DisplayName="tickerSoundStart"))
	FGsSchemaSoundResDataRow tickerSoundStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="종료 연출 티커 사운드", DisplayName="tickerSoundClear"))
	FGsSchemaSoundResDataRow tickerSoundClear;
public:
	FGsSchemaAgitDungeonRaidConfig();
	
protected:
	bool operator== (const FGsSchemaAgitDungeonRaidConfig& __Other) const
	{
		return FGsSchemaAgitDungeonRaidConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAgitDungeonRaidConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAgitDungeonRaidConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAgitDungeonRaidConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAgitDungeonRaidConfigRow() : Super()
	{
	}
	
	const FGsSchemaAgitDungeonRaidConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAgitDungeonRaidConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaAgitDungeonRaidConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAgitDungeonRaidConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAgitDungeonRaidConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAgitDungeonRaidConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild/Agit");
	}
	
};
