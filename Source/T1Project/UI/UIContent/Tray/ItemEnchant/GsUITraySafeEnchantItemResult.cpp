// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySafeEnchantItemResult.h"
#include "Components/WidgetSwitcher.h"
#include "T1Project.h"


void UGsUITraySafeEnchantItemResult::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUITraySafeEnchantItemResult::NativeDestruct()
{
	if (true == _onFinishedAnim.IsBound())
	{
		_onFinishedAnim.Clear();
	}

	Super::NativeDestruct();
}

void UGsUITraySafeEnchantItemResult::SetEffectData(int32 InSwitcherIndex)
{
	_tickerSwitcher->SetActiveWidgetIndex(InSwitcherIndex);
}

void UGsUITraySafeEnchantItemResult::OnFinishedAnimation()
{
	//GSLOG(Warning, TEXT("[UGsUITraySafeEnchantItemResult::OnFinishedAnimation()]"));
	if (true == _onFinishedAnim.IsBound())
	{
		_onFinishedAnim.Broadcast(true);
	}

}