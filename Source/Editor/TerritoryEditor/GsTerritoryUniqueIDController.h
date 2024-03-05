
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

//ue4

//territory eidtor
#include "Define/GsTerritorySpawnDefine.h"
#include "Define/GsTerritoryZoneShapeDefine.h"

//RSQL 저장 기능이 없어서 예전 방식으로 저장
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "Define/GsTerritoryUniqueTable.h"


const FString INSTANCE_MISSING = TEXT("Territory editor가 없습니다.");
const FString ZONE_UNIQUE_ID_FULL = TEXT("Zone의 UniqueID를 모두다 사용했습니다.");
const FString LINE_UNIQUE_ID_FULL = TEXT("Line의 UniqueID를 모두다 사용했습니다.");
const FString NPC_UNIQUE_ID_FULL = TEXT("Npc의 UniqueID를 모두다 사용했습니다.");

class UGsTable;
class UWorld;

//bit20 max
const uint32 TERRITORY_INSTANCE_ID_MAX = 1048575;
//const uint32 TERRITORY_INSTANCE_ID_MAX = 3;

//서버 레벨 내의 Territory actor의 instance id를 Object 단위로 관리하는 클래스
class FGsTerritoryObjectInstanceIDController
{
private:
	UPROPERTY(Transient)
		bool _isAscending = false;
	UPROPERTY(Transient)
		int64 _maxInstanceID = 1;
	UPROPERTY(Transient)
		TSet<uint32> _usedInstanceIDs;
	UPROPERTY(Transient)
		TSet<uint32> _unusedInstanceIDs;

public:
	virtual ~FGsTerritoryObjectInstanceIDController();

public:
	void InitUnusedInstanceIDs();
	
	//Instance id와 object type을 조합하여 Unique ID를 생성한다. 
	static int64 CombineTerritoryUniqueID(EGsTerritoryObjType inObjType, uint32 inInstanceID);
	static int64 GetInstanceIDFromUniqueID(EGsTerritoryObjType inObjType, uint32 inInstanceID);
	void Initailize();	
	void InvalidMaxInstanceID();

private:
	bool TryGenerateAscendingInstanceID(uint32& outInstanceID);
	bool TryGenerateInstanceID(uint32& outInstanceID);

	//get set
public:
	bool TryGenerateUniqueID(EGsTerritoryObjType inObjType, int64& outTerritoryUniqueID, int64& outInstanceID);
	bool TryGenerateUniqueID(EGsTerritoryObjType inObjType, uint32& inInstanceID, int64& outTerritoryUniqueID, int64& outInstanceID);
	bool Add(uint32 inInstanceID);
	bool AddForce(uint32 inInstanceID);
	bool Remove(uint32 inInstanceID);
	bool IsCanGenerateUniqueID(uint32& inInstanceID, bool inIsForceCheck=false);	
	void SetAscending(bool inIsAscending){ _isAscending = inIsAscending;}	
	bool IsAscending(){return _isAscending;}
	bool IsFull();
	int64 GetMaxInstanceID() {return _maxInstanceID;}
};

//맵내 서버 레벨의 TerritoryActor Instance ID를 관리하는 클래스
class FGsTerritoryServerLevelInstanceIDController
{
public:	
	TMap<EGsTerritoryObjType, TSharedPtr<FGsTerritoryObjectInstanceIDController>> _instanceIDController;

public:	
	virtual ~FGsTerritoryServerLevelInstanceIDController();

public:
	void Initialize(ULevel* inLevel);
	bool InitAllTerritoryActorsInServerLevel(ULevel* inLevel);
	void InitAllTerritoryActorsUnusedInstanceIDs();
	bool TryGenerateUniqueID(EGsTerritoryObjType inObjType, int64& outTerritoryUniqueID, int64& outInstanceID);
	bool TryGenerateUniqueID(EGsTerritoryObjType inObjType, uint32& inInstanceID, int64& outTerritoryUniqueID, int64& outInstanceID);
	bool Add(EGsTerritoryObjType inType, uint32 inInstanceID);
	bool AddForce(EGsTerritoryObjType inType, uint32 inInstanceID);
	bool Remove(EGsTerritoryObjType inType, uint32 inInstanceID);
	bool IsCanGenerateUniqueID(EGsTerritoryObjType inType);
	bool IsCanGenerateUniqueID(EGsTerritoryObjType inType, uint32& inInstanceID, bool inIsForceCheck = false);
	bool IsAscendingGenerateUniqueID(EGsTerritoryObjType inType);

private:
	FGsTerritoryObjectInstanceIDController* GetObjectUniqueIDController(EGsTerritoryObjType inType);
};