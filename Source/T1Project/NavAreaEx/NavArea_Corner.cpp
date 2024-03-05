// Fill out your copyright notice in the Description page of Project Settings.


#include "NavArea_Corner.h"


UNavArea_Corner::UNavArea_Corner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultCost = 50.0f;
	DrawColor = FColor(255, 102, 102);
}
