#include "GsMovementRunner.h"
#include "ActorEx/GsCharacterBase.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"

#define DEBUG_LOC 1
#if DEBUG_LOC
#include "DrawDebugHelpers.h"
#endif

//-----------------------------------------------------------------------------------------------------------------
// FGsMovementRunner
//-----------------------------------------------------------------------------------------------------------------
void FGsMovementRunner::InitMoveComponent(class AGsCharacterBase* character)
{
	_moveComponent = Cast<UCharacterMovementComponent>(character->GetMovementComponent());
	check(_moveComponent);
}
void FGsMovementRunner::SetRunnerLocation(const FVector& inLoc)
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		_owner->SetActorLocation(inLoc);
#if DEBUG_LOC
		DrawDebugSphere(_owner->UObject::GetWorld(), inLoc, 50.0f, 12, FColor::Cyan, false, 0.10f, 0, 1.0f);
#endif
	}
	else
	{
		character->SetActorLocation(inLoc);
	}
}

const FVector FGsMovementRunner::GetRunnerLocation() const
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		return _owner->GetActorLocation();
	}
	return character->GetActorLocation();
}

const FRotator FGsMovementRunner::GetRunnerRotation() const
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		return _owner->GetActorRotation();
	}
	return character->GetActorRotation();
}

const FVector FGsMovementRunner::GetRunnerFeetLocation()
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		return _owner->GetActorLocation();
	}
	InitMoveComponent(character);
	return _moveComponent->GetActorFeetLocation();
}

const FVector FGsMovementRunner::GetRunnerForwardVector()
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		return GetRunnerRotation().Vector();
	}
	return character->GetActorForwardVector();
}

void FGsMovementRunner::GetSimpleCollisionCylinder(float& CollisionRadius, float& CollisionHalfHeight)
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		FGsGameObjectDataCreature* creatureData = _owner->GetCastData<FGsGameObjectDataCreature>();
		CollisionRadius = creatureData->GetScaledCapsuleRadius();
		CollisionHalfHeight = creatureData->GetScaledCapsuleHalfHeight();
		return;
	}

	character->GetSimpleCollisionCylinder(CollisionRadius, CollisionHalfHeight);
}

void FGsMovementRunner::SetRunnerRotation(const FRotator& NewRotation)
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		_owner->SetActorRotation(NewRotation);
		return;
	}
	character->SetActorRotation(NewRotation);
}

void FGsMovementRunner::RequestDirectMove(const FVector& inDelta)
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr == character)
	{
		FVector toLoc = _owner->GetActorLocation() + inDelta;
		_owner->SetActorLocation(toLoc);
#if DEBUG_LOC
		DrawDebugSphere(_owner->UObject::GetWorld(), toLoc, 50.0f, 12, FColor::Purple, false, 0.10f, 0, 1.0f);
#endif
	}
	else
	{
		InitMoveComponent(character);
		_moveComponent->RequestDirectMove(inDelta, false);
	}
}

void FGsMovementRunner::StopActive()
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr != character)
	{
		InitMoveComponent(character);
		_moveComponent->StopActiveMovement();

		if (auto Anim = character->GetAnim())
		{
			Anim->IsMovingAttack= false;
		}		
	}
}

void FGsMovementRunner::RequestMovingAttack()
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if(nullptr != character)
	{
		if (auto Anim = character->GetAnim())
		{
			Anim->IsMovingAttack = true;
		}
	}
}

void FGsMovementRunner::StopMovingAttack()
{
	AGsCharacterBase* character = _owner->GetCharacter();
	if (nullptr != character)
	{
		if (auto Anim = character->GetAnim())
		{
			Anim->IsMovingAttack = false;
		}
	}
}
