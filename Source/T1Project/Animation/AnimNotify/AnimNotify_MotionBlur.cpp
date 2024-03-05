// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_MotionBlur.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsCameraPostProcessComponent.h"


void UAnimNotify_MotionBlur::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AGsCharacterLocalPlayer* actor = Cast<AGsCharacterLocalPlayer>(MeshComp->GetOwner());
	if (nullptr == actor)
	{
		return;
	}

	if(UGsCameraPostProcessComponent* ActorComp = actor->FindComponentByClass<UGsCameraPostProcessComponent>())
	{
		ActorComp->StartMotionBlurNotify(_motionBlurTime, IgnorePreBlur);
	}
}
