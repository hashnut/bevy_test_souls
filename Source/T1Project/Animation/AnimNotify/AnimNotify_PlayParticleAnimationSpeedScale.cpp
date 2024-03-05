// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayParticleAnimationSpeedScale.h"
#include "Particles/ParticleSystemComponent.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "AnimNotify_PlayParticleParam.h"

UParticleSystemComponent* UAnimNotify_PlayParticleAnimationSpeedScale::SpawnParticleSystem(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	UParticleSystemComponent* particle = Super::SpawnParticleSystem(MeshComp, Animation);
	if (particle)
	{
		particle->InstanceParameters = InstanceParameters;

		if (UAnimInstance* anim = MeshComp->GetAnimInstance())
		{
			if (FAnimMontageInstance* activeMontage = anim->GetActiveMontageInstance())
			{
				float playRate = activeMontage->GetPlayRate();

#if WITH_EDITOR
				// 에디터에서 Animation Scale 변경 확인용 처리
				if (MeshComp->GetWorld()->IsEditorWorld())
				{
					UAnimMontage* animMontage = activeMontage->Montage;
					// 슬롯이 여러개 있더라도 상위 슬롯 정보를 참조한다.
					if (animMontage->SlotAnimTracks.Num() > 0)
					{
						for (FAnimSegment& el : animMontage->SlotAnimTracks[0].AnimTrack.AnimSegments)
						{
							if (el.StartPos > activeMontage->GetPosition())
							{
								break;
							}
							playRate = el.AnimPlayRate;
						}
					}
				}
#endif
				particle->CustomTimeDilation = playRate;

			}
		}
	}

	return particle;
}