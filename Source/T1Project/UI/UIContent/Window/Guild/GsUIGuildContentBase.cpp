// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentBase.h"


void UGsUIGuildContentBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();
}
