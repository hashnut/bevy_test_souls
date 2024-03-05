// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GsTabSwitcher.generated.h"

class UTextBlock;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGsTabSwitcher : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class T1PROJECT_API IGsTabSwitcher
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FText GetTabTitle() const = 0;
	virtual FLinearColor GetTabColor() = 0;

	virtual FLinearColor GetTabBaseColor() const = 0;
	virtual FLinearColor GetTabSelectColor() const = 0;
	virtual FLinearColor GetTabDisabledColor() const = 0;
};
