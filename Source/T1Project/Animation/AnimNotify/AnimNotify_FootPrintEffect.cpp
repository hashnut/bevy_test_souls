// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_FootPrintEffect.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "ActorEx/GsCharacterPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequenceBase.h"

#include "ActorEx/GsPhysicalMaterial.h"
#include "GameObject/Define/EGsFootPrintMethod.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "CollisionQueryParams.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#if WITH_EDITOR
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#endif


void UAnimNotify_FootPrintEffect::PostLoad()
{
	Super::PostLoad();

	_RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void UAnimNotify_FootPrintEffect::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAnimNotify_FootPrintEffect, RotationOffset))
	{
		_RotationOffsetQuat = FQuat(RotationOffset);
	}
}
#endif

UParticleSystemComponent* UAnimNotify_FootPrintEffect::SpawnParticleSystem(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(MeshComp->GetOwner());
	if (nullptr == actor)
	{
		return UAnimNotify_PlayParticleEffect::SpawnParticleSystem(MeshComp, Animation);
	}

	if (actor->IsHidden())
	{
		return nullptr;
	}

	UParticleSystem* footPrintEffect = nullptr;
	FVector spawnLocation;

	const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);

	do
	{
		// 콜리전이 있으면 (수면 이펙트 등)
		if (UGsPhysicalMaterial* overlapedM = actor->GetFootPrintOverlapedMaterial())
		{
			if (overlapedM->FootPrintEffect)
			{
				footPrintEffect = overlapedM->FootPrintEffect;

				FRotator R = actor->GetFootPrintOverlapedRotation();
				FVector rotatedUpVector = R.RotateVector(FVector::UpVector);
				FVector planeCenter = actor->GetFootPrintOverlapedLocation();
				float spawnZ = planeCenter.Z - (rotatedUpVector.X * (MeshTransform.GetLocation().X - planeCenter.X) + rotatedUpVector.Y * (MeshTransform.GetLocation().Y - planeCenter.Y)) / rotatedUpVector.Z;
				spawnLocation = FVector(MeshTransform.GetLocation().X, MeshTransform.GetLocation().Y, spawnZ);
				spawnLocation += overlapedM->LocationOffset;
				break;
			}
		}

		// 바닥 위에 있지 않으면 출력하지 않는다.
		UCharacterMovementComponent* moveComponent = Cast<UCharacterMovementComponent>(actor->GetMovementComponent());
		if (nullptr == moveComponent || false == moveComponent->CurrentFloor.bWalkableFloor)
		{
			break;
		}

		// 트리거 액터 안에 있을 때
		if (actor->IsValidFootPrintTrigger())
		{
			if (actor->GetFootPrintTriggerEffect())
			{
				footPrintEffect = actor->GetFootPrintTriggerEffect();
				spawnLocation = MeshTransform.TransformPosition(LocationOffset);
				break;
			}
			else if (actor->GetFootPrintTriggerMaterial())
			{
				UGsPhysicalMaterial* triggerM = actor->GetFootPrintTriggerMaterial();
				footPrintEffect = triggerM->FootPrintEffect;
				spawnLocation = MeshTransform.TransformPosition(LocationOffset + triggerM->LocationOffset);
				break;
			}
		}

		if (EGsFootPrintMethod::MAP == actor->GetDefaultFootPrintMethod())
		{
			if (actor->GetDefaultFootPrintEffect())
			{
				footPrintEffect = actor->GetDefaultFootPrintEffect();
				spawnLocation = MeshTransform.TransformPosition(LocationOffset);
				break;
			}
			else if (actor->GetDefaultFootPrintMaterial())
			{
				UGsPhysicalMaterial* defaultM = actor->GetDefaultFootPrintMaterial();
				footPrintEffect = defaultM->FootPrintEffect;
				spawnLocation = MeshTransform.TransformPosition(LocationOffset + defaultM->LocationOffset);
				break;
			}
		}

		if (EGsFootPrintMethod::MATERIAL == actor->GetDefaultFootPrintMethod())
		{
			TArray<UPhysicalMaterial*> complexPhysicalMaterials = actor->GetBasedMovement().MovementBase->GetBodyInstance()->GetComplexPhysicalMaterials();
			for (UPhysicalMaterial* complexPhysicalMaterial : complexPhysicalMaterials)
			{
				if (UGsPhysicalMaterial* physicalM = Cast<UGsPhysicalMaterial>(complexPhysicalMaterial))
				{
					if (physicalM->FootPrintEffect)
					{
						footPrintEffect = physicalM->FootPrintEffect;
						spawnLocation = MeshTransform.TransformPosition(LocationOffset + physicalM->LocationOffset);
						break;
					}
				}
			}

			UPhysicalMaterial* simplePhyM = actor->GetBasedMovement().MovementBase->GetBodyInstance()->GetSimplePhysicalMaterial();
			if (UGsPhysicalMaterial* physicalM = Cast<UGsPhysicalMaterial>(simplePhyM))
			{
				if (physicalM->FootPrintEffect)
				{
					footPrintEffect = physicalM->FootPrintEffect;
					spawnLocation = MeshTransform.TransformPosition(LocationOffset + physicalM->LocationOffset);
					break;
				}
			}

			FHitResult hitResult = moveComponent->CurrentFloor.HitResult;
			if (hitResult.PhysMaterial.IsValid())
			{
				TWeakObjectPtr<UPhysicalMaterial> phyM = hitResult.PhysMaterial;
				if (UGsPhysicalMaterial* physicalM = Cast<UGsPhysicalMaterial>(phyM))
				{
					if (physicalM->FootPrintEffect)
					{
						footPrintEffect = physicalM->FootPrintEffect;
						spawnLocation = MeshTransform.TransformPosition(LocationOffset + physicalM->LocationOffset);
						break;
					}
				}
			}
		}

		footPrintEffect = PSTemplate;
		spawnLocation = MeshTransform.TransformPosition(LocationOffset);

	} while (0);

	UParticleSystemComponent* ReturnComp = nullptr;
	if (Attached)
	{
		ReturnComp = UGameplayStatics::SpawnEmitterAttached(footPrintEffect, MeshComp, SocketName, spawnLocation - MeshTransform.GetLocation(), RotationOffset, Scale,
			EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease, true);
	}
	else
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(spawnLocation);
		SpawnTransform.SetRotation(MeshTransform.GetRotation() * _RotationOffsetQuat);
		SpawnTransform.SetScale3D(Scale);

		ReturnComp = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), footPrintEffect, SpawnTransform, true, EPSCPoolMethod::AutoRelease);
	}

	return ReturnComp;
}