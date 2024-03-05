/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UMaterialInterface;
#include "GsSchemaSanctumConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sanctum/BsGsSanctumConfig"))
struct DATACENTER_API FGsSchemaSanctumConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="전투 전 1차 노티 알림(초)", DisplayName="battleFirstNotifySec"))
	int32 battleFirstNotifySec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="전투 전 성소 수호탑 디스폰 알림(초)", DisplayName="battlePrepareSec"))
	int32 battlePrepareSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소 점령전 스케줄", DisplayName="battleStartCron"))
	TArray<FString> battleStartCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소 점령전 진행 시간", DisplayName="battelDurationSec"))
	int32 battelDurationSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소 점령전 종료 시 완료로 판정할 완성도", DisplayName="completeBonusGaugePercent"))
	int32 completeBonusGaugePercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소 점령전 종료 시 버프 획득 연출용", DisplayName="battleVictoryFxAbnormalityId"))
	FGsSchemaAbnormalitySetRow battleVictoryFxAbnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="지어지지 않은 성소 머테리얼", DisplayName="sanctumMaterial"))
	UMaterialInterface* sanctumMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="pathIconNormal"))
	FSoftObjectPath pathIconNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="pathIconWin"))
	FSoftObjectPath pathIconWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tickerSoundStart"))
	FGsSchemaSoundResDataRow tickerSoundStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tickerSoundEnd"))
	FGsSchemaSoundResDataRow tickerSoundEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tickerSoundWin"))
	FGsSchemaSoundResDataRow tickerSoundWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="종료 전 1차 알림", DisplayName="battleBeforeEndNotify1Sec"))
	int32 battleBeforeEndNotify1Sec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="종료 전 2차 알림", DisplayName="battleBeforeEndNotify2Sec"))
	int32 battleBeforeEndNotify2Sec;
public:
	FGsSchemaSanctumConfig();
	
protected:
	bool operator== (const FGsSchemaSanctumConfig& __Other) const
	{
		return FGsSchemaSanctumConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSanctumConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSanctumConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSanctumConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSanctumConfigRow() : Super()
	{
	}
	
	const FGsSchemaSanctumConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSanctumConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaSanctumConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSanctumConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSanctumConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSanctumConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sanctum");
	}
	
};
