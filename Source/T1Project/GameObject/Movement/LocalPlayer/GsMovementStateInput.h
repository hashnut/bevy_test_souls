#pragma once


#include "../GsMovementState.h"


// 서버와 이동타입 정보를 맞추기 타입중 인풋이동쪽을 처리하기 위한 대표이다..
// 추가적으로 인풋을 통한점프등이 추가되면 FGsMovementMethodInputJump 식으로 해당 클래스를 세분화해 나가면된다.

class UGsGameObjectLocalPlayer;
class AGsCharacterLocalPlayer;

class T1PROJECT_API FGsMovementStateInput : public FGsMovementState
{
	FVector2D						_moveScale		= FVector2D::ZeroVector;
	AGsCharacterLocalPlayer*		_characterLocal = nullptr;
	UGsGameObjectLocalPlayer*		_owner			= nullptr;
	mutable FVector					_destination	= FVector::ZeroVector;
	TFunction<void()>				_oppositeDirecitonHandle;
	
public:
	static float	CacheChangeScaleRateRunWalk;

public:
	FGsMovementStateInput() : FGsMovementState(EGsMovementState::INPUT) {}

	//---------------------------------------------------------------------------------------------
	// FGsMovementState
	//---------------------------------------------------------------------------------------------
public:
	virtual const MotionType GetMotionType() const override;

public:
	virtual int16 GetDirNetConverted() const override;
	virtual float GetFallingSpeed() const override;
	virtual const FVector& GetLocation() const override;
	virtual const FVector& GetDestination() const override;

	//---------------------------------------------------------------------------------------------
	// TGsState
	//---------------------------------------------------------------------------------------------
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float inDeltaTime) override;


	//---------------------------------------------------------------------------------------------
	// This
	//---------------------------------------------------------------------------------------------
public:
	// 초기화
	void	Initialize(UGsGameObjectLocalPlayer* In_local);

	void	Start(UGsGameObjectLocalPlayer* inOwner, const FVector& inDir);
	void	Start(UGsGameObjectLocalPlayer* inOwner);

	void	SetHandleOpposideDirection(TFunction<void()> inFunc) { _oppositeDirecitonHandle = inFunc; }

protected:
	virtual void Stop(class UGsGameObjectCreature* inOwner, bool inSendFsmMessage) override;

public:
	void	SetMoveScale(const FVector2D& inVal) { _moveScale = inVal; }
	void	SetCharacterLocal(AGsCharacterLocalPlayer* inCharacter) { _characterLocal = inCharacter; }

public:	
	void	SetActorRotationFromJoyAxis(AGsCharacterLocalPlayer*, const FVector& inDir);
	void	SetDirecitonFromMoveScale(const FVector2D& inMoveScale);
private:
	void	ClearMoveScale() { _moveScale = FVector2D::ZeroVector; }
	void	AddMovementInput(AGsCharacterLocalPlayer* inTarget);

public:
	bool	IsRunnableScaleValue() const;
	bool	IsMovableScaleValue() const	{ return _moveScale.SizeSquared() > 0.f;}
};
