// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_FairySkillEffect.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_FairySkillEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AGsActorFieldFairy* actor = Cast<AGsActorFieldFairy>(MeshComp->GetOwner()))
	{
		if (Attached)
		{
			if (actor->AnimNoti_AttachFairySkillEffect(SocketName, Scale, LocationOffset, RotationOffset, MeshComp))
				return;
		}

		if (auto pstemplate = actor->GetPSTemplate())
			PSTemplate = pstemplate;

		Super::Notify(MeshComp, Animation);
	}
}