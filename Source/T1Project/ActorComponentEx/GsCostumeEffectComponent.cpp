// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCostumeEffectComponent.h"
#include "ActorEx/GsCharacterPlayer.h"


UGsCostumeEffectComponent::UGsCostumeEffectComponent()
{
#if (WITH_EDITOR)
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	bTickInEditor = true;
#endif
}