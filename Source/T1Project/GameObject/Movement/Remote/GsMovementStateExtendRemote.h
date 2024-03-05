#pragma once
#include "../GsMovementState.h"

//-------------------------------------------------------------------------------------------------
// FGsMovementState 클래스 인터페이스를 Remote 클래스들을 위해 확장한다.
//-------------------------------------------------------------------------------------------------

class UGsMovementDebugShapeComponent;

class T1PROJECT_API FGsMovementStateExtendRemote : public FGsMovementState
{
protected:
	FVector			_lastPacketPos	= FVector::ZeroVector;
	FVector			_lastPacketDir	= FVector::ZeroVector;
	FVector			_lastPacketDest = FVector::ZeroVector;

	FVector			_velocity		= FVector::ZeroVector;

	UGsMovementDebugShapeComponent* _debugShape = nullptr;

public:
	FGsMovementStateExtendRemote(EGsMovementState inEnumType) :
		FGsMovementState(inEnumType) {}
	virtual ~FGsMovementStateExtendRemote() = default;

public:
	virtual void SetNetDirection(const FVector& inDireciton)		{ _lastPacketDir = inDireciton; }
	virtual void SetNetDestination(const FVector& inDestination)	{ _lastPacketDest = inDestination; }
	virtual void SetNetLocation(const FVector& inLocation)			{ _lastPacketPos = inLocation; }
			void SetVelocity(const FVector& inVelocity)				{ _velocity = inVelocity; }

public:	
	void		 SetDebugShapeComponent(UGsMovementDebugShapeComponent* inShape) { _debugShape = inShape; }
	
public:
	virtual const MotionType GetMotionType() const override { return MotionType::MAX; }

public:
	virtual void Enter() override {}
	virtual void Exit() override {}
	virtual void Update(float In_deltaTime) override {}

	const FVector&			GetVelocity() const								{ return _velocity; }
	const FVector&			GetNetDirection() const							{ return _lastPacketDir; }
	virtual int16			GetDirNetConverted() const override				{ return 0; }
	virtual float			GetFallingSpeed() const override				{ return 0.0f;}
	virtual const FVector&	GetDirection() const override					{ _direction = (GetDestination() - _location).GetSafeNormal2D(); return _direction;}
	virtual const FVector& GetDestination() const override					{ return _lastPacketDest; }

	virtual bool			CanTransition(EGsMovementState inType) override		{ return false; }
};

