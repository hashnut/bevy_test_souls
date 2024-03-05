// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICoolTimeEffectProgressBar.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"


void UGsUICoolTimeEffectProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	SetProgress(0.f);
}

void UGsUICoolTimeEffectProgressBar::SetProgress(float InRate)
{
	_progressBar->SetPercent(InRate);

	_thumbPos.X = 0.f;
	_thumbPos.Y = (InRate - 1.f) * _barHeight;

	_panelThumb->SetRenderTranslation(_thumbPos);

	if (0 < InRate)
	{
		_panelThumb->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_panelThumb->SetVisibility(ESlateVisibility::Hidden);
	}
}
