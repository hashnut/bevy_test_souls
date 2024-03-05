// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Spawn/GsSchemaSpawnElement.h"

	//packet for client spawn
struct FGsClientSpawnParamContainer
{
public:
	TArray<TSharedPtr<FGsSpawnParam>> _paramList;

public:
	void Clear()
	{
		_paramList.Empty();
	}
};

struct FGsSpawnElementContainer
{
public:
	TArray<FGsSchemaSpawnElement> _paramList;

public:
	void Clear()
	{
		_paramList.Empty();
	}
};

struct FGsSpawnedGameObjContainer
{
public:
	TSet<int64> _spawnMap;

public:
	void Clear()
	{
		_spawnMap.Empty();
	}
};