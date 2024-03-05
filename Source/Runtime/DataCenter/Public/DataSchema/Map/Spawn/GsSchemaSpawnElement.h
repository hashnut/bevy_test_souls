/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Spawn/GsSchemaSpawnObjectShowHide.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Quest/EGsVisibleType.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpawnElement.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsSpawnElement"))
struct DATACENTER_API FGsSchemaSpawnElement
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Territory unique id", DisplayName="SpawnId"))
	int64 SpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="memberType"))
	NpcSpawnGroupMemberType memberType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpawnType"))
	CreatureSpawnType SpawnType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Npc Id, reference npc table", DisplayName="CreatureId"))
	int32 CreatureId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="How many spawn count at the same time", DisplayName="SpawnCount"))
	int32 SpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RandomPos"))
	bool RandomPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnLevel"))
	int32 spawnLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Pos"))
	FVector Pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dir"))
	int32 Dir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Milliseconds", DisplayName="RespawnInterval"))
	int32 RespawnInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Spawn Group Type이 RANDOMRATE 인 경우, Zone에 배치된 몬스터에게 스폰 확률을 설정", DisplayName="spawnRate"))
	int32 spawnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupLimitRange"))
	float groupLimitRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ClientGameId"))
	int64 ClientGameId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="InitVisible"))
	bool InitVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="VisibleType"))
	EGsVisibleType VisibleType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="QuestCondition"))
	TArray<FGsSchemaSpawnObjectShowHide> QuestCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="offensiveType"))
	OffensiveType offensiveType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sightRadius"))
	float sightRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="aggroLink"))
	bool aggroLink;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="AiOptimizeOnOff"))
	bool AiOptimizeOnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isFixedLocation"))
	bool isFixedLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skipTerrainCheck"))
	bool skipTerrainCheck;
public:
	FGsSchemaSpawnElement();
	
protected:
	bool operator== (const FGsSchemaSpawnElement& __Other) const
	{
		return FGsSchemaSpawnElement::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
