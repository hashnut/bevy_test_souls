#include "GsLocalCharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "ActorEx/GsCharacterBase.h"

void UGsLocalCharacterMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice /*= 0.f*/, const FVector& MoveDelta /*= FVector::ZeroVector*/)
{
	_bBlockingHit = Hit.bBlockingHit;
	Super::HandleImpact(Hit, TimeSlice, MoveDelta);
}


