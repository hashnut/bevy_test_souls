
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdPlayModeState : public FGsTerrEdModeStateBase
{
public:
	virtual ~FGsTerrEdPlayModeState(){}

public:
	virtual void Enter() override;
	virtual void Exit() override;
};