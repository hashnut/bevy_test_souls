#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/ICursor.h"
#include "Framework/Application/IInputProcessor.h"


class T1PROJECT_API FGsJoystickInputProcessor : public IInputProcessor
{
public:
	DECLARE_DELEGATE(FOnJoystickPressed);
	DECLARE_DELEGATE(FOnJoystickReleased);

protected:
	bool _isPress = false;

	TArray<FKey> _keyCodes;

public:
	FOnJoystickPressed OnJoystickPressed;
	FOnJoystickReleased OnJoystickReleased;

public:
	FGsJoystickInputProcessor();
	virtual ~FGsJoystickInputProcessor() {}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
};