
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GsTerrEdModeStateBase.h"
#include "GsTerrEdModeStateManagerBase.h"


class TERRITORYEDITOR_API FGsTerrEdModeStateManager : public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:	
	FGsTerrEdModeStateManager();
	virtual ~FGsTerrEdModeStateManager(){}	
};