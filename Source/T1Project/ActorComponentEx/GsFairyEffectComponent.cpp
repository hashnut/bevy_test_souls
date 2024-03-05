#include "GsFairyEffectComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

//const FName SpawnEffectName = TEXT("HitEffectSocket");

UGsFairyEffectComponent::UGsFairyEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UParticleSystemComponent* UGsFairyEffectComponent::OnEffect(
	UParticleSystem* In_particle,
	USceneComponent* In_targetcomponent,
	bool In_isAutoDestroy,
	EPSCPoolMethod In_poolType,
	EAttachLocation::Type In_locationType,
	FVector In_location ,
	FRotator In_rotate,
	FVector effectScale,
	FName In_attachPointName)
{
	if (!In_particle || !GetOwner() || GetOwner()->IsHidden()) return nullptr;

	auto targetcomponent = In_targetcomponent;

	if (!targetcomponent)
		targetcomponent = GetOwner()->GetRootComponent();

	if (!targetcomponent) return nullptr;

	UParticleSystemComponent* resVal = nullptr;
	if (targetcomponent != nullptr)
	{
		resVal =
			UGameplayStatics::SpawnEmitterAttached(
				In_particle,
				targetcomponent,
				In_attachPointName,
				In_location,
				In_rotate,
				effectScale,
				In_locationType,
				In_isAutoDestroy,
				In_poolType,
				true);
	}

	return resVal;
}

