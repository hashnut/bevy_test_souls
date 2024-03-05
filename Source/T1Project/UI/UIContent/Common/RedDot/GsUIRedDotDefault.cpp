// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRedDotDefault.h"


void UGsUIRedDotDefault::SetRedDot(bool bIsRedDot, EGsIconRedDotType InType, int32 InNumber)
{
	SetVisibility(bIsRedDot ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}
