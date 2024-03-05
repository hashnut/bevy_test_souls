// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_FairyTeleportEffect.h"

#include "ActorEx/GsActorFieldFairy.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Components/SkeletalMeshComponent.h"


void UAnimNotify_FairyTeleportEffect::Notify(USkeletalMeshComponent* In_meshComp, UAnimSequenceBase* Animation)
{
	if (AGsActorFieldFairy* actorFairy = Cast<AGsActorFieldFairy>(In_meshComp->GetOwner()))
	{
		if (ETeleportEffectType::MAX == _fairyTeleportEffect)
		{
			return;
		}
		else
		{
			actorFairy->OnFairyTeleportParticleEffect(
				_fairyTeleportEffect,
				SocketName,
				Scale,
				LocationOffset,
				RotationOffset,
				In_meshComp,
				Attached
			);

			// 만약 떠나는 경우라면, 페어리 액터를 숨겨준다
			if (ETeleportEffectType::DEPART == _fairyTeleportEffect)
			{
				actorFairy->SetActorHiddenInGame(true);
			}
		}
	}
}