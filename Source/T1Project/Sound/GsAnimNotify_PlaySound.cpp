// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAnimNotify_PlaySound.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

#include "UObjectBaseUtility.h"
#include "Components/AudioComponent.h"

#include "T1Project.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "GsSoundPlayer.h"

#include "GameObject/Define/EGsFootPrintMethod.h"

#include "DataSchema/Sound/GsSchemaSoundResData.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsPhysicalMaterial.h"
#include "GameObject/Define/GsGameObjectDefine.h"


void UGsAnimNotify_PlaySound::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	if (Sound)
	{
		if (Sound->IsLooping())
		{
			return;
		}

		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			if (true == IsCharacter(MeshComp))
			{
				uint64 audioComponentID = soundPlayer->PlaySoundAnimNotify(Sound, MeshComp, AttachName,
					bFollow, VolumeMultiplier, PitchMultiplier, bSpeedScale);
				soundPlayer->SetAudioParameter(EGsAudioParameterType::FootPrint, static_cast<float>(GetFootSoundType(MeshComp)),
					UAudioComponent::GetAudioComponentFromID(audioComponentID));
			}
			else
			{
				soundPlayer->PlaySoundAnimNotify(Sound, MeshComp, AttachName,
					bFollow, VolumeMultiplier, PitchMultiplier, bSpeedScale);
			}
		}
		else
		{
			if (bFollow)
			{
				UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, AttachName,
					FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
			}
			else
			{
				UGameplayStatics::SpawnSoundAtLocation(MeshComp->GetWorld(),
					Sound, MeshComp->GetComponentLocation(), FRotator::ZeroRotator, VolumeMultiplier, PitchMultiplier);
			}
		}		
	}
}

bool UGsAnimNotify_PlaySound::IsCharacter(class USkeletalMeshComponent* MeshComp)
{
	AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(MeshComp->GetOwner());
	if (nullptr == actor)
		return false;

	return true;
}

EGsFootSoundType UGsAnimNotify_PlaySound::GetFootSoundType(class USkeletalMeshComponent* MeshComp)
{
	AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(MeshComp->GetOwner());
	if (nullptr == actor)
		return EGsFootSoundType::Dirt;

	EGsFootSoundType materialType = EGsFootSoundType::Dirt;

	// 콜리전이 있으면 (수면 이펙트 등)
	if (UGsPhysicalMaterial* overlapedM = actor->GetFootPrintOverlapedMaterial())
	{
		materialType = overlapedM->_materialType;
	}
	else
	{
		if (UGsPhysicalMaterial* triggerM = actor->GetFootPrintTriggerMaterial())
		{
			materialType = triggerM->_materialType;
		}
		else if (EGsFootPrintMethod::MAP == actor->GetDefaultFootPrintMethod())
		{
			if (UGsPhysicalMaterial* defaultM = actor->GetDefaultFootPrintMaterial())
			{
				materialType = defaultM->_materialType;
			}
		}
		else if (EGsFootPrintMethod::MATERIAL == actor->GetDefaultFootPrintMethod())
		{
			// 바닥 위에 있으면 (발먼지)
			if (UCharacterMovementComponent* moveComponent = Cast<UCharacterMovementComponent>(actor->GetMovementComponent()))
			{
				if (moveComponent->CurrentFloor.bWalkableFloor)
				{
					if (UGsPhysicalMaterial* physicalM = Cast< UGsPhysicalMaterial>(moveComponent->CurrentFloor.HitResult.PhysMaterial))
					{
						materialType = physicalM->_materialType;
					}
				}
			}
		}
	}

	return materialType;
}