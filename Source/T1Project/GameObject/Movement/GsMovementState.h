#pragma once

#include "CoreMinimal.h"
#include "../classes/GsState.h"
#include "Shared/Client/SharedEnums/SharedMoveEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

UENUM(BlueprintType)
enum class EGsMovementState : uint8
{
	INPUT = 0,
	AUTO = 1,
	STOP = 2,
	REMOTE = 3,
	SPECTATOR = 4,
};

// 서버와 이동타입 정보를 맞추기 위한 최소타입
// 패킷에 필요한 정보 Interface 및 실제 타입별 처리코드를 담당한다.
// MovementHandler에선 해당 상태 타입을 확인하여 현재 상황을 파악할수 있는 용이점도 있다
class T1PROJECT_API FGsMovementState : public TGsState <EGsMovementState>
{
	friend class FGsMovementHandlerBase;

	DECLARE_EVENT(FGsMovementState, MessageDelegator);
	
protected:
	StatValue			_moveSpeed = 600; 
	bool				_dirty = false;
	mutable FVector		_direction = FVector::ZeroVector;
	mutable FVector		_location = FVector::ZeroVector;
	MessageDelegator	_stopFuncRouter;

public:
	explicit FGsMovementState(EGsMovementState inEnumType) : TGsState<EGsMovementState>(inEnumType) {}

public:
	virtual const MotionType GetMotionType() const = 0;

public:
	virtual void SetDirection(const FVector& inDir);
	virtual void SetLocation(const FVector& inCurrent) { _location = inCurrent; }
	virtual void SetDirty(bool inDirty) { _dirty = inDirty; }
	virtual void SetMoveSpeed(StatValue inMoveSpeed);

protected:
	virtual void Stop(class UGsGameObjectCreature* inOwner, bool inSendFsmMessage);

public:
	virtual int16 GetDirNetConverted() const = 0;
	virtual float GetFallingSpeed() const = 0;
	virtual const FVector& GetLocation() const { return _location; }
	virtual const FVector& GetDestination() const = 0;
	virtual const FVector& GetDirection() const;
	virtual bool GetDirty() const { return _dirty; }
	virtual StatValue GetMoveSpeed() const { return _moveSpeed; }

public:
	MessageDelegator& GetStopFuncRouter() { return _stopFuncRouter; }
};