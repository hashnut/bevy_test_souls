// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Common/Item/GsUIItemIconSlotEffect.h"
#include "Components/WidgetSwitcher.h"


void UGsUIItemIconSlotEffect::NativeConstruct()
{
	Super::NativeConstruct();

}

void UGsUIItemIconSlotEffect::NativeDestruct()
{

	Super::NativeDestruct();
}

void UGsUIItemIconSlotEffect::SetSlotEffect(const EGsItemIconSlotEffectType InType)
{
	if (EGsItemIconSlotEffectType::None == InType)
		return;

	uint8 indexType = (uint8)InType - 1;
	_effectSwitcher->SetActiveWidgetIndex(indexType);
}

void UGsUIItemIconSlotEffect::SetSlotEffectByIndex(int32 InIndex)
{
	if (INDEX_NONE >= InIndex)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		_effectSwitcher->SetActiveWidgetIndex(InIndex);
	}
}