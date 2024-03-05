// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsObjectState.generated.h"


/**
 * UObject 상속받는 State. GsObjectStateMng의 관리를 받음.
 */
UCLASS()
class T1PROJECT_API UGsObjectState : public UObject
{
	GENERATED_BODY()
	
private:
	uint8 StateEnumType = 0;

public:
	virtual void Init(uint8 InType) { StateEnumType = InType; }
	virtual void Close() {}

public:
	virtual void Enter() {};
	virtual void Exit() {};
	virtual void Update(float In_deltaTime) {};

public:
	uint8 GetType() const { return StateEnumType; }
};
