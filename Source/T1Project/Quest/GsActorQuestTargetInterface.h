// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GsActorQuestTargetInterface.generated.h"

class USphereComponent;
class UWidgetComponent;
class UGsUIBillboardInteractionButton;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGsActorQuestTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class T1PROJECT_API IGsActorQuestTargetInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	virtual UStaticMeshComponent* GetQuestTargetMark(){return nullptr; }
};
