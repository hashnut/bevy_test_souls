// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDGameRating.h"

void UGsUIHUDGameRating::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);
}

bool UGsUIHUDGameRating::UpdateManagerTick(float InDeltaTime)
{
	Super::UpdateManagerTick(InDeltaTime);

	if (_endTime > 0 && _endTime - FApp::GetCurrentTime() <= 0)
	{
		_endTime = 0;
		SetVisibility(ESlateVisibility::Collapsed);
	}
	return _endTime > 0;

}
void UGsUIHUDGameRating::SetVisibleTime(float sec)
{
	_endTime = FApp::GetCurrentTime() + sec;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
