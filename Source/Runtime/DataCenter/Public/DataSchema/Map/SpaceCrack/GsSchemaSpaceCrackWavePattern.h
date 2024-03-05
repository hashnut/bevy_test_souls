/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterSpawnRate.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Wave/GsSchemaMonsterWave.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedMapEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackWavePattern.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackWavePattern"))
struct DATACENTER_API FGsSchemaSpaceCrackWavePattern
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="patternType"))
	SpaceCrackActivatePatternType patternType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rate"))
	int32 rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterWaveList"))
	TArray<FGsSchemaMonsterWave> monsterWaveList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bossList"))
	TArray<FGsSchemaMonsterSpawnRate> bossList;
public:
	FGsSchemaSpaceCrackWavePattern();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackWavePattern& __Other) const
	{
		return FGsSchemaSpaceCrackWavePattern::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
