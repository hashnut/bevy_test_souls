// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GsActorInteractionInterface.generated.h"

class USphereComponent;
class UWidgetComponent;
class UGsUIBillboardInteractionButton;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGsActorInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class T1PROJECT_API IGsActorInteractionInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ClickInteraction() {}
	virtual UWidgetComponent* GetInteractionWidgetComponent() { return nullptr; }
	virtual UGsUIBillboardInteractionButton* GetInteractionButton() const { return nullptr; }
	virtual void SetUseInteractionButton(bool inUse) {}
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) {}	
};
