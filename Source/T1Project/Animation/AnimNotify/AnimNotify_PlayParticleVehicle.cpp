// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_PlayParticleVehicle.h"
#include "ActorEx/GsCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


UParticleSystemComponent* UAnimNotify_PlayParticleVehicle::SpawnParticleSystem(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	// 숨김 처리 되어 있으면 스폰하지 않는다
	if (MeshComp && false == MeshComp->IsVisible())
	{
		return nullptr;
	}

	UParticleSystemComponent* ReturnComp = nullptr;
	if (PSTemplate)
	{
		ReturnComp = Super::SpawnParticleSystem(MeshComp, Animation);
	}

	if (AGsCharacterBase* castActor = Cast<AGsCharacterBase>(MeshComp->GetOwner()))
	{
		// UAnimNotify_PlayParticleEffect::SpawnParticleSystem 내부와 동일하지만 외부 액터에 설정된 리소스를 참조한다.
		if (castActor->_customParticle)
		{
			if (castActor->_customParticle->IsLooping())
			{
				return ReturnComp;
			}

			if (Attached)
			{
#if RAON_CUSTOM_ETC
				ReturnComp = UGameplayStatics::SpawnEmitterAttached(castActor->_customParticle, MeshComp, SocketName, LocationOffset, RotationOffset, Scale, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease, true);
#else
				ReturnComp = UGameplayStatics::SpawnEmitterAttached(castActor->_customParticle, MeshComp, SocketName, LocationOffset, RotationOffset, Scale);
#endif
			}
			else
			{
				const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(MeshTransform.TransformPosition(LocationOffset));
				SpawnTransform.SetRotation(MeshTransform.GetRotation() * RotationOffset.Quaternion());
				SpawnTransform.SetScale3D(Scale);
#if RAON_CUSTOM_ETC
				ReturnComp = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), castActor->_customParticle, SpawnTransform, true, EPSCPoolMethod::AutoRelease);
#else
				ReturnComp = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), castActor->_customParticle, SpawnTransform);
#endif
			}
		}
	}

	return ReturnComp;
}
