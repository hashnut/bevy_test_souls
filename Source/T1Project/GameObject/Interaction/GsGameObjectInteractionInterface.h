// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/Interface.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsGameObjectInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGsGameObjectInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class T1PROJECT_API IGsGameObjectInteractionInterface
{
	GENERATED_BODY()

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void UpdateInteractionColliderSize(float In_rate = 0.0f) {}
	virtual void DrawDebugInteractionRange() {}
	virtual void OnClickInteract(bool In_isByWidget) {}	

public:
	virtual void SetInteractionActive(bool In_isActive, int In_index = 0) {}
	virtual bool GetIsActiveInteraction() { return false; }

public:
	virtual bool IsContentsUnlock() { return true; }
};