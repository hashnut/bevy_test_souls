
#include "GsJoystickInpuProcessor.h"


FGsJoystickInputProcessor::FGsJoystickInputProcessor()
{

}

void FGsJoystickInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{

}

bool FGsJoystickInputProcessor::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{
	if (_keyCodes.Contains(InAnalogInputEvent.GetKey()) && 
		InAnalogInputEvent.GetAnalogValue() < FLT_EPSILON  && InAnalogInputEvent.GetAnalogValue() > -FLT_EPSILON)
	{
		_keyCodes.Remove(InAnalogInputEvent.GetKey());

		if (_isPress && _keyCodes.Num() == 0)
		{
			OnJoystickReleased.ExecuteIfBound();
			_isPress = false;
			return false;
		}
	}

	if (false == _keyCodes.Contains(InAnalogInputEvent.GetKey()) && 
		(InAnalogInputEvent.GetAnalogValue() > FLT_EPSILON  || InAnalogInputEvent.GetAnalogValue() < -FLT_EPSILON))
	{
		_keyCodes.Emplace(InAnalogInputEvent.GetKey());

		if (false == _isPress)
		{
			OnJoystickPressed.ExecuteIfBound();
			_isPress = true;
		}
	}

	return false;
}

