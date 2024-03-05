
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

class TERRITORYEDITOR_API FGsTerrEdModeStateBase
{
protected:
	float _time = 0;
	bool _isTick = false;

public:
	FGsTerrEdModeStateBase(){}
	virtual~FGsTerrEdModeStateBase(){}
	virtual void Initialize(){}
	virtual void Tick(float inTick){}
	virtual void Enter(){ _time = 0;}
	virtual void Exit(){}
	void SetTickEnable(bool inEnable) {
		_isTick = inEnable;
	}
};