// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define/GsTerritoryShapeDefine.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryLevelEvent
{
public:
	static int64 _npcID;
	static bool _bCreateNpc;
	static EGsTerritorySpawnType _spawnType;

private:
	FDelegateHandle _npcTableDoubleClickHandle;

private:
	FDelegateHandle _beignTerritoryActorTransfrom;
	FDelegateHandle _endTerritoryActorTransfrom;

public:
	virtual ~FGsTerritoryLevelEvent();
	
public:
	void Initialize();
	void Clear();


public:
	void BindNpcTable();
	void UnbindNpcTable();
	void OnDoubleClickNpcTable(FName inKey);

public:
	void BindTerritoryActorTransform();
	void UnbindTerritoryActorTransform();
	void OnBeginTerritoryActorTransform(UObject& inObj);
	void OnEndTerritoryActorTransfrom(UObject& inObj);
};
