/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAgitConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/Agit/BSGsAgitConfig"))
struct DATACENTER_API FGsSchemaAgitConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영지 대여 필요 기사단 레벨", DisplayName="rentalRequireGuildLevel"))
	int32 rentalRequireGuildLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rentalRequireMemberGrade"))
	GuildMemberGradeType rentalRequireMemberGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영지 입장 시 소모할 아이템", DisplayName="enterItem"))
	FGsSchemaItemCommonRow enterItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영지에서 복귀 시 소모할 아이템", DisplayName="returnItem"))
	FGsSchemaItemCommonRow returnItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대여에 필요한 골드", DisplayName="rentalGold"))
	int32 rentalGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대여 기간_일", DisplayName="rentalPeriodD"))
	int32 rentalPeriodD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대여 연장에 필요한 골드", DisplayName="rentalExtensionGold"))
	int32 rentalExtensionGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대여 연장 기간_일", DisplayName="rentalExtensionPeriodD"))
	int32 rentalExtensionPeriodD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대여 연장 가능한 최소 일자_일", DisplayName="rentalExtensionMinD"))
	int32 rentalExtensionMinD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대여 기간 만료 전 기사단원 티커 알림 시간_분", DisplayName="rentalExpirationAlertMList"))
	TArray<int32> rentalExpirationAlertMList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기사단 영지 맵", DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영지 퇴장 시 이동 맵", DisplayName="exitMapId"))
	FGsSchemaMapDataRow exitMapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영지 퇴장 시 이동 맵의 스팟", DisplayName="exitSpotIdList"))
	TArray<int32> exitSpotIdList;
public:
	FGsSchemaAgitConfig();
	
protected:
	bool operator== (const FGsSchemaAgitConfig& __Other) const
	{
		return FGsSchemaAgitConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAgitConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAgitConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAgitConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAgitConfigRow() : Super()
	{
	}
	
	const FGsSchemaAgitConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAgitConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaAgitConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAgitConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAgitConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAgitConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild/Agit");
	}
	
};
