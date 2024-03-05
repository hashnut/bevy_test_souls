// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISaveBatteryMode.h"
#include "Components/WidgetSwitcher.h"

void UGsUISaveBatteryMode::SetMode(int32 InIndex)
{
	_switcherMode->SetActiveWidgetIndex(InIndex);
}

