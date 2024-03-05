// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Common/GsUISwipeCircle.h"
#include "Components/WidgetSwitcher.h"


void UGsUISwipeCircle::OnShow()
{
	_switcher->SetActiveWidgetIndex(1);
}

void UGsUISwipeCircle::OnHide()
{
}

void UGsUISwipeCircle::SetCircleActive(const bool InIsActive)
{
	_switcher->SetActiveWidgetIndex(InIsActive ? 0 : 1);
}