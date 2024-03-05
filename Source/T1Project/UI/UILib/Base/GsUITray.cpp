// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITray.h"
#include "Management/ScopeGlobal/GsUIManager.h"

void UGsUITray::SetHideVisibility()
{
	auto curVisibility = GetVisibility();
	if (curVisibility == ESlateVisibility::Collapsed ||
		curVisibility == ESlateVisibility::Hidden) return;

	_restoreVisibility = curVisibility;
	PauseAllAnimations();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITray::RestoreShowVisibility()
{
	SetVisibility(_restoreVisibility);
	RestartAllAnimations();
}

void UGsUITray::NativeConstruct()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->AddTicker(this);
	}
	Super::NativeConstruct();
}

void UGsUITray::NativeDestruct()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->RemoveTicker(this);
	}
	Super::NativeDestruct();
}
