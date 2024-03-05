// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/GsUITrayTickerInvasionNoti.h"

void UGsUITrayTickerInvasionNoti::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_finishDelegate.BindDynamic(this, &UGsUITrayTickerInvasionNoti::OnAnimationFinish);
	BindToAnimationFinished(_tickerAnimation, _finishDelegate);
}

void UGsUITrayTickerInvasionNoti::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(_tickerAnimation);
}

void UGsUITrayTickerInvasionNoti::OnAnimationFinish()
{
	Close(true);
}