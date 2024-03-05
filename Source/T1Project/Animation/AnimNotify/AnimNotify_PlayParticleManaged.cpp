// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayParticleManaged.h"
#include "ActorEx/GsCharacterBase.h"
#include "../GsAnimInstanceState.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystemComponent.h"

UParticleSystemComponent* UAnimNotify_PlayParticleManaged::SpawnParticleSystem(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	UParticleSystemComponent* particle = Super::SpawnParticleSystem(MeshComp, Animation);
	if (particle)
	{
		if (AGsCharacterBase* actor = Cast<AGsCharacterBase>(MeshComp->GetOwner()))
		{
			UGsAnimInstanceState* anim = actor->GetAnim();
			anim->AddAnimNotifyParticle(_Type, particle);
		}

		// 외부 프로퍼티에서 설정할수 있지만, 제작자 실수로 설정하지않았을경우 대비
		// 이설정을 하지 않으면 Deactivate() 호출을 해도 정상 동작 하지 않습니다.
		for (FParticleEmitterInstance* el : particle->EmitterInstances)
		{
			if (el)
			{
				el->bKillOnDeactivate = true;
			}
		}
	}

	return particle;
}