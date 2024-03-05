/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterSpawnRateGroup.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedMapEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterWave.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Wave/BSGsMonsterWave"))
struct DATACENTER_API FGsSchemaMonsterWave
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="zako"))
	FGsSchemaMonsterSpawnRateGroup zako;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="elite"))
	FGsSchemaMonsterSpawnRateGroup elite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="waveConditionType"))
	WaveConditionType waveConditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionParamList"))
	TArray<int32> conditionParamList;
public:
	FGsSchemaMonsterWave();
	
protected:
	bool operator== (const FGsSchemaMonsterWave& __Other) const
	{
		return FGsSchemaMonsterWave::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
