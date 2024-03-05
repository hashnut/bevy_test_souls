// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GsEditableInterface.generated.h"

class UGsUIWidgetBase;
class UWidget;
class UUserWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGsEditableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class T1PROJECT_API IGsEditableInterface
{
	GENERATED_BODY()

protected:
	bool _useEditableCheck{ false };
	TWeakObjectPtr<UInputComponent> _inputComponent;
	TWeakObjectPtr<UGsUIWidgetBase> _widget;

	TArray<UWidget*> _editableWidget;
	bool _includeEditableWidget{ false };

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	void Initialize();
	void Finalize();

	void InputInitialize(TWeakObjectPtr<UGsUIWidgetBase> ownRoot);
	void InputFinalize(TWeakObjectPtr<UGsUIWidgetBase> ownRoot);

	void KeyProcInitialize(TWeakObjectPtr<UGsUIWidgetBase> ownRoot);

public:
	void EnableEditableInput();
	void DisableEditableInput();

	bool IsEditableWidgetInclude();

	TArray<UWidget*> LoadEditableWidgetRecursive(TWeakObjectPtr<UUserWidget> ownRoot);

protected:
	virtual void SetInputComponent(UInputComponent* inputComponent);

private:
	bool FindEditableWidgetResursive(TWeakObjectPtr<UUserWidget> ownRoot);
};
