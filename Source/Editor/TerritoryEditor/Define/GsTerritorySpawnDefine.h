// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

//T1 Project
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "Quest/EGsVisibleType.h"
#include "Quest/EGsQuestCheckType.h"

//Territory editor
#include "GsTerritoryZoneShapeDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "GsTerritorySpawnDefine.generated.h"
/**
 *
 */

UENUM(BlueprintType)
enum class EGsTerritoryEventType : uint8
{
	SpawnLocation,
	Patrol,
};

class AGsTerritoryActor;
class AGsTerritoryZone;

USTRUCT(BlueprintType)
struct FGsTerritoryGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
		TArray<AGsTerritoryZone*> TerritoryZoneArray;

public:
	virtual ~FGsTerritoryGroup() {}
};

USTRUCT(BlueprintType)
struct FGsTerritoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Territory base")
		int64 UniqueID = -1;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Territory base")
		bool CanDebug = false;

public:
	virtual ~FGsTerritoryInfoBase() {}
};


USTRUCT(BlueprintType)
struct FGsTerritoryZoneInfo : public FGsTerritoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool byClient = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Territory zone")
		int32 GroupID;
	UPROPERTY()
		FGsTerritoryZoneShapeData Shape;

public:
	virtual ~FGsTerritoryZoneInfo() {}
};

USTRUCT(BlueprintType)
struct FGsTerrEdSpawnObjectShowHide
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone")
		FString StartConditionQuestId = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone")
		FString EndConditionQuestId = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory zone")
		EGsQuestCheckType QuestCheckType;

};

USTRUCT(BlueprintType)
struct FGsTerritorySpawnInfo : public FGsTerritoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		int32 TargetID;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "TerritoryTableData")
		EGsTerritorySpawnType Type = EGsTerritorySpawnType::Npc;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "TerritoryTableData")
		int32 SpawnLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TerritoryTableData")
		NpcSpawnGroupMemberType SpawnGroupMemberType = NpcSpawnGroupMemberType::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		int32 MaxSpawnNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		float InitSpawnDelay = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		float RespawnInterval = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		int SpawnRate = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		float groupLimitRange =0.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "TerritoryTableData")
		FVector RelativeLocation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "TerritoryTableData")
		FVector Rotation;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool CanRandomLocationSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool InitVisible = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		EGsVisibleType VisibleType = EGsVisibleType::Show;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		TArray<FGsTerrEdSpawnObjectShowHide> QuestCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		OffensiveType offensiveType = OffensiveType::USE_NPC_DATA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		float sightRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool isAggroLink = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool AiOptimizeOnOff = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool isFixedLocation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerritoryTableData")
		bool SkipTerrainCheck = false;	

public:
	virtual ~FGsTerritorySpawnInfo() {}
};

USTRUCT(BlueprintType)
struct FGsTerritoryCompareCaution
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
		AGsTerritoryActor* Source;
	UPROPERTY(Transient)
		AGsTerritoryActor* Dest;

public:
	FString ToString();
};