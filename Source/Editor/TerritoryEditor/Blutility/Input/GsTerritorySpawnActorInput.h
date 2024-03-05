// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Territory editor

#include "Define/GsTerritorySpawnDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Quest/EGsVisibleType.h"
#include "GsTerritorySpawnActorInput.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGsTerritorySpawnShapeType : uint8
{
	Random,
};

USTRUCT(BlueprintType)
struct FGsTerritorySpawnInput
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		int32 TargetID;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		int32 SpawnLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		NpcSpawnGroupMemberType SpawnGroupMemberType = NpcSpawnGroupMemberType::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		int32 MaxSpawnNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		int32 InitSpawnDelay = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		int32 RespawnInterval = 60;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		float groupLimitRange = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		bool CanRandomLocationSpawn = false;		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		bool InitVisible = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		EGsVisibleType VisibleType = EGsVisibleType::Show;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainTableData")
		TArray<FGsTerrEdSpawnObjectShowHide> QuestCondition;
};

USTRUCT(BlueprintType)
struct FGsTerritoryNpcInput : public FGsTerritorySpawnInput
{
	GENERATED_BODY()
};

USTRUCT()
struct FGsTerritoryNpcIDInfo
{
	GENERATED_BODY()

public:
	FString desc;
	int32 id;
};

USTRUCT()
struct FGsTerritoryKeyDescInfo
{
	GENERATED_BODY()

public:
	FString desc;
	uint64 id;
};

USTRUCT(BlueprintType)
struct FGsTerritoryPropInput : public FGsTerritorySpawnInput
{
	GENERATED_BODY()
};

USTRUCT()
struct FGsTerritorySpawnIDInfo
{
	GENERATED_BODY()

public:
	FString desc;
	int32 id;
};

USTRUCT(BlueprintType)
struct FGsTerritoryZoneInput
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool byClient = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGsTerritoryZoneShapeType Shape = EGsTerritoryZoneShapeType::Circle;
};