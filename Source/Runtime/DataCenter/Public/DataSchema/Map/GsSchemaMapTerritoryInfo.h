/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/GameObject/ObjectClass/Data/GsEnvCondition.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapTerritoryInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapTerritoryInfo"))
struct DATACENTER_API FGsSchemaMapTerritoryInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="territoryId"))
	int64 territoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="territoryType"))
	TerritoryType territoryType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fenceId"))
	int64 fenceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "byClient"))
	bool byClient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition="(territoryType == TOWN) || (territoryType == SAFE_ZONE)", DisplayName="sound"))
	FGsSchemaSoundResDataRow sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition="(territoryType == Env)", DisplayName="envConditionArray"))
	TArray<FGsEnvCondition> envConditionArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition = "(territoryType == SOUND)", DisplayName="questSound"))
	TArray<FGsQuestSound> questSoundArray;
public:
	FGsSchemaMapTerritoryInfo();
	
protected:
	bool operator== (const FGsSchemaMapTerritoryInfo& __Other) const
	{
		return FGsSchemaMapTerritoryInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
