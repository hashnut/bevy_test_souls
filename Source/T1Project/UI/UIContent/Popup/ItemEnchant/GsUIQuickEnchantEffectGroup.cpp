// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIQuickEnchantEffectGroup.h"
#include "Components/CanvasPanel.h"


void UGsUIQuickEnchantEffectGroup::NativeConstruct()
{
	Super::NativeConstruct();

	_enchantSuccessEffectPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIQuickEnchantEffectGroup::NativeDestruct()
{
	_enchantSuccessEffectPanel->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeDestruct();
}
