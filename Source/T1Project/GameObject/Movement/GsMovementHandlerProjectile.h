// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsMovementHandlerBase.h"
#include "../Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
 * Projectile 전용 이동 관련 처리 클래스
 */
class T1PROJECT_API FGsMovementHandlerProjectile: 
	public FGsMovementHandlerBase
{
private:
	// 프로젝타일형 MoveComponent는 다른 클래스이다.
	class AGsActorProjectile* _projectileActor = nullptr;
	class UProjectileMovementComponent* _projectileMoveComponent = nullptr;

	float _speed;
	FVector _destPos;

public:
	FGsMovementHandlerProjectile() = default;
	virtual ~FGsMovementHandlerProjectile()	= default;

	//------------------------------------------------------------------------------------------------
	// FGsMovementHandlerBase
	//------------------------------------------------------------------------------------------------
public:
	virtual void Initialize(UGsGameObjectBase* inOwner) override;
	virtual void InitializeActor(class AActor* Owner) override;
	virtual void Update(float inDelta) override;

public:
	virtual void InitSpeed(int32 inMoveSpeed, int32 inWalkSpeed) override;

// 간단한 이동형 클래스 이기 때문에 FGsMovementNetReceiver 상속까진 필요 없을것 같다.
	void ReceveDesPos(const FVector& inDesPos);
	void ReceveSpeed(float inSpeed);
};
