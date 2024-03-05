// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayParticleParam.h"
#include "ActorEx/GsCharacterBase.h"
#include "../GsAnimInstanceState.h"
#include "ParticleEmitterInstances.h"


UParticleSystemComponent* UAnimNotify_PlayParticleParam::SpawnParticleSystem(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	UParticleSystemComponent* particle = Super::SpawnParticleSystem(MeshComp, Animation);
	if (particle)
	{
		particle->InstanceParameters = InstanceParameters;
	}

	return particle;
}