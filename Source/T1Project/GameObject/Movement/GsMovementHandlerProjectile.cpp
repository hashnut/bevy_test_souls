// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMovementHandlerProjectile.h"
#include "GsMovementHandlerBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ActorEx/GsActorProjectile.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "UTIL/GsDir.h"

//------------------------------------------------------------------------------------------------
// FGsMovementHandlerBase
//------------------------------------------------------------------------------------------------
void FGsMovementHandlerProjectile::Initialize(UGsGameObjectBase* inOwner)
{
	check(inOwner);
	_owner = inOwner;
}

void FGsMovementHandlerProjectile::InitializeActor(class AActor* Owner)
{
	if (nullptr != Owner)
	{
		_projectileActor = Cast<AGsActorProjectile>(Owner);
		_projectileMoveComponent = _projectileActor->GetMoveComponent();
		_projectileMoveComponent->InitialSpeed = _speed;
	}
}

void FGsMovementHandlerProjectile::Update(float inDelta)
{
	if (_projectileActor)
	{
		FVector velocity = _destPos - _projectileActor->GetActorLocation();
		FVector dir = velocity.GetSafeNormal();
		FRotator rotator = dir.Rotation();
		
		_projectileActor->SetActorRotation(rotator);
		_projectileActor->SetProjectileSpeed(_speed);
		// _projectileMoveComponent->ComputeVelocity(velocity, inDelta);
	}
}

void FGsMovementHandlerProjectile::InitSpeed(int32 inMoveSpeed, int32 inWalkSpeed)
{
	_speed = inMoveSpeed;
}


void FGsMovementHandlerProjectile::ReceveDesPos(const FVector& inDesPos)
{
	_destPos = inDesPos;
}

void FGsMovementHandlerProjectile::ReceveSpeed(float inSpeed)
{
	_speed = inSpeed;
}