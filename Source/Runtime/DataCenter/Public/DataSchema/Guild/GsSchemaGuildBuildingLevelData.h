/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "GsSchemaGuildBuildingLevelData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildBuildingLevelData"))
struct DATACENTER_API FGsSchemaGuildBuildingLevelData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="빌딩 레벨"))
	int32 level;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaPassivitySetRow passivityId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="요구 길드 레벨"))
	int32 needLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="빌딩 레벨업 비용(길드 운영비 사용)"))
	int32 levelUpCost;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="레벨업에 드는 시간(초)"))
	int32 levelUpSecond;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="버프 활성화 재화 타입"))
	CurrencyType buffActiveCostType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="버프 활성화 재화량"))
	int32 buffActiveCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildStatValue", Tooltip))
	int32 guildStatValue;
	
public:
	FGsSchemaGuildBuildingLevelData();
	
protected:
	bool operator== (const FGsSchemaGuildBuildingLevelData& __Other) const
	{
		return FGsSchemaGuildBuildingLevelData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
