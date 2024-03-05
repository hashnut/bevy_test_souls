// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardPropTouch.h"

void UGsUIBillboardPropTouch::SetVisibility(ESlateVisibility InVisibility)
{
	if (_bIsHide)
	{
		_backupVisibility = InVisibility;
	}

	Super::SetVisibility(InVisibility);
}
