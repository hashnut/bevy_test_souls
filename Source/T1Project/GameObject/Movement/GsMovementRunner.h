#pragma once

#include "CoreMinimal.h"


class UGsGameObjectCreature;
class UCharacterMovementComponent;
//-----------------------------------------------------------------------------------------------------------------
 // FGsMovementRunner
 //-----------------------------------------------------------------------------------------------------------------
class FGsMovementRunner
{
public:
	FGsMovementRunner() = default;
	FGsMovementRunner(UGsGameObjectCreature* inOwner) : _owner(inOwner) {}
private:
	class UGsGameObjectCreature* _owner = nullptr;
	UCharacterMovementComponent* _moveComponent = nullptr;

private:
	void InitMoveComponent(class AGsCharacterBase* character);

public:
	void SetRunnerLocation(const FVector& inLoc);
	const FVector GetRunnerLocation() const;
	const FRotator GetRunnerRotation() const;

	const FVector GetRunnerFeetLocation();
	const FVector GetRunnerForwardVector();

	void GetSimpleCollisionCylinder(float& CollisionRadius, float& CollisionHalfHeight);

public:
	void SetRunnerRotation(const FRotator& NewRotation);

public:
	void RequestDirectMove(const FVector& inDelta);
	void StopActive();
	void RequestMovingAttack();
	void StopMovingAttack();
};