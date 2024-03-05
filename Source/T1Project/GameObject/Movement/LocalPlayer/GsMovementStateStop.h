#pragma once

#include "../GsMovementState.h"

// 서버와 이동타입 정보를 맞추기 타입중 정지쪽을 처리하기 위한 클래스
// 이동이라고 예외처리하지 않고 동일한 인터페이싱으로 접근가능하도록 제어하며
// GsMovementHandler에서 타입처리함으로써 정지상태를 직관적으로 알수 있음(Auto이동 or Input 이동이 없는 상태)
class AGsCharacterLocalPlayer;
class T1PROJECT_API FGsMovementStateStop : public FGsMovementState
{
	AGsCharacterLocalPlayer*	_characterLocal = nullptr;

public:
	FGsMovementStateStop() : FGsMovementState(EGsMovementState::STOP) {}

	//---------------------------------------------------------------------------------------------
	// FGsMovementState
	//---------------------------------------------------------------------------------------------
	void	SetCharacterLocal(AGsCharacterLocalPlayer* inCharacter) { _characterLocal = inCharacter; }

public:
	virtual const MotionType GetMotionType() const override { return MotionType::STOP; }

	virtual void Stop(class UGsGameObjectCreature* inOwner, bool inSendFsmMessage) override {};

public:
	virtual int16 GetDirNetConverted() const override;
	virtual float GetFallingSpeed() const override;
	virtual const FVector& GetDirection() const override;
	virtual const FVector& GetLocation() const override;
	virtual const FVector& GetDestination() const override;


	//---------------------------------------------------------------------------------------------
	// TGsState
	//---------------------------------------------------------------------------------------------
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float inDeltaTime) override;
};