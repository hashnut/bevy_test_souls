// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/SpiritShot/GsUITrayTickerSpiritShotEnchant.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Animation/UMGSequencePlayer.h"


void UGsUITrayTickerSpiritShotEnchant::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, TEXT("OnExitAnimation"));
	BindToAnimationFinished(_inAnim, FWidgetAnimationDynamicEvent(animationDelegate));
}

void UGsUITrayTickerSpiritShotEnchant::SetText(FText&& InText)
{
	_trayTickerText->SetText(MoveTemp(InText));

	PlayAnimation(_inAnim);
}
