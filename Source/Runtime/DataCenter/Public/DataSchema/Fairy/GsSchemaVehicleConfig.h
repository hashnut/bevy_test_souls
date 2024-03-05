/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UCurveFloat;
#include "GsSchemaVehicleConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsVehicleConfig"))
struct DATACENTER_API FGsSchemaVehicleConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="탈 것 상태로 전환하기 위한 비전투 이동 유지 시간 (1.0000 = 1초, 5.000 = 5초)", DisplayName="nonCombatMoveTime"))
	float nonCombatMoveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="탈것 발동 시작 애니메이션(남)", AllowedClasses="AnimMontage", DisplayName="vehicleMoveStart_M"))
	FSoftObjectPath vehicleMoveStart_M;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="탈것 발동 시작 애니메이션(여)", AllowedClasses="AnimMontage", DisplayName="vehicleMoveStart_F"))
	FSoftObjectPath vehicleMoveStart_F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="페어리 SpringArm Length 조절 정보", DisplayName="vehicleFairyTargetLength"))
	UCurveFloat* vehicleFairyTargetLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="vehicleFairyPosition"))
	FVector vehicleFairyPosition;
public:
	FGsSchemaVehicleConfig();
	
protected:
	bool operator== (const FGsSchemaVehicleConfig& __Other) const
	{
		return FGsSchemaVehicleConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaVehicleConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaVehicleConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaVehicleConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaVehicleConfigRow() : Super()
	{
	}
	
	const FGsSchemaVehicleConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaVehicleConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaVehicleConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaVehicleConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaVehicleConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableVehicleConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
};
