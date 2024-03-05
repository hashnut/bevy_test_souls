// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/SpiritShot/GsUITooltipSpiritShotProb.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void UGsUITooltipSpiritShotProb::InvalidateAll(FText InDescText, FText&& InMinRate, FText&& InMaxRate, FText&& InFallRate)
{
	_descText->SetText(InDescText);
	_minSuccessRate->SetText(MoveTemp(InMinRate));
	_maxSuccessRate->SetText(MoveTemp(InMaxRate));
	_fallRate->SetText(MoveTemp(InFallRate));
}
