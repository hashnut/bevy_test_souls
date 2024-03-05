// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAccelationButton.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"

void UGsAccelationButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (_button)
	{
		//마우스 버튼이 떼어졌을때 Release를 호출하려면 Focusable을 꺼야한다.
		//_button->focus(false);

		_button->OnClicked.Clear();
		_button->OnClicked.AddDynamic(this, &UGsAccelationButton::Clicked);
		_button->OnPressed.Clear();
		_button->OnPressed.AddDynamic(this, &UGsAccelationButton::Pressed);
		_button->OnReleased.Clear();
		_button->OnReleased.AddDynamic(this, &UGsAccelationButton::Released);
	}

	_shortTouchEvent.Clear();
	_longTouchEvent.Clear();

	float start;
	float end;

	if (_curve)
	{
		_curve->GetTimeRange(start, end);
		_maxTime = end;

		UE_LOG(LogTemp, Log, TEXT("MaxTime time : %f"), _maxTime);
	}

	_isPressed = false;

	_currentTime = 0;
	_accumTime = 0;
}

void UGsAccelationButton::BeginDestroy()
{
	if (_button)
	{	
		_button->OnClicked.Clear();
		_button->OnPressed.Clear();
		_button->OnReleased.Clear();
	}		

	_shortTouchEvent.Clear();
	_longTouchEvent.Clear();

	Super::BeginDestroy();
}

void UGsAccelationButton::NativeTick(const FGeometry& inMyGeometry, float inDeltaTime)
{
	Super::NativeTick(inMyGeometry, inDeltaTime);

	if (false == _isPressed)
	{
		return;
	}	

	_currentTime += inDeltaTime;
	_accumTime += inDeltaTime;

	if (_currentTime > _tickTime)
	{
		_currentTime = 0.0f;

		if (_curve)
		{
			if (_accumTime > _maxTime)
			{
				_accelation = _curve->GetFloatValue(_maxTime);
			}
			else
			{
				_accelation = _curve->GetFloatValue(_accumTime);
			}
		}

		_longTouchEvent.Broadcast(_accelation);

		UE_LOG(LogTemp, Log, TEXT("Long touch event"));
	}	
}

void UGsAccelationButton::Clicked()
{	
	UE_LOG(LogTemp, Log, TEXT("Clicked"));
}

void UGsAccelationButton::Pressed()
{
	_isPressed = true;
	_currentTime = 0;
	_accelation = 0;
	_accumTime = 0;

	_shortTouchEvent.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("Pressed"));
}

void UGsAccelationButton::Released()
{
	_accelation = 0;	
	_isPressed = false;
	_currentTime = 0;
	_accumTime = 0;

	UE_LOG(LogTemp, Log, TEXT("Released"));
}

void UGsAccelationButton::SetIsEnabled(bool inEnable)
{
	if (inEnable)
	{
		ButtonEnableEvent();
	}
	else
	{
		ButtonDisableEvent();
	}
}

void UGsAccelationButton::ButtonEnableEvent_Implementation()
{

}

void UGsAccelationButton::ButtonDisableEvent_Implementation()
{

}
