// Fill out your copyright notice in the Description page of Project Settings.


#include "NavArea_Road.h"


UNavArea_Road::UNavArea_Road(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultCost = 0.5f;
	DrawColor = FColor(0, 102, 51);
}
